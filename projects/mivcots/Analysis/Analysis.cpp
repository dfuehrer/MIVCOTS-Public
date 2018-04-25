#include "Analysis.h"
#include <wx/log.h>
#include "AnalysisReturnCodes.h"



AnalysisParent::AnalysisParent()
{
	analysisFinishedCounterInt.store(0, std::memory_order_relaxed);
}


AnalysisParent::~AnalysisParent()
{
}



int AnalysisParent::init(
	sharedCache<CarData*> * boxCache,
	sharedCache<CarData*> * carCache,
	endpoint<CarData*> * updateQueue,
	CarPool * carPool,
	std::string configFileName
)
{
	int returnCode = 0;
	if (boxCache != nullptr) {
		this->boxCache = boxCache;
	}
	else {
		wxLogError("Analysis Parent - Init Function: boxCache Pointer is Null");
		returnCode |= ERR_BOX_CACHE_PTR_IS_NULL;
	}
	if (carCache != nullptr) {
		this->carCache = carCache;
	}
	else {
		wxLogError("Analysis Parent - Init Function: carCache Pointer is Null");
		returnCode |= ERR_CAR_CACHE_PTR_IS_NULL;
		return returnCode;
	}
	if (updateQueue != nullptr) {
		this->updateQueue = updateQueue;
	}
	else {
		wxLogError("Analysis Parent - Init Function: updateQueue Pointer is Null");
		returnCode |= ERR_UPDATE_QUEUE_PTR_IS_NULL;
		return returnCode;
	}
	if (carPool != nullptr) {
		this->carPool = carPool;
	}
	else {
		wxLogError("Analysis Parent - Init Function: carPool Pointer is Null");
		returnCode |= ERR_CAR_CACHE_PTR_IS_NULL;
		return returnCode;
	}
	if (!(configFileName.empty())) {
		this->configFileName = configFileName;
	}
	else {
		wxLogError("Analysis Parent - Init Function: Config File Name is Empty");
		returnCode |= WARN_CONFIG_FILENAME_IS_EMPTY;

	}


	return returnCode;
}

int AnalysisParent::start()
{
	analysisParentThread = std::thread(&AnalysisParent::runAnalysisThread, this);
	return 0;
}

int AnalysisParent::stop()

{
	//TODO: Make sure to call delete on all the stuff that start() calls new on.
	while (analysisChildVector.size() > 0) {	// TODO: This should be a for loop
		analysisChildVector.back()->stop();
	}
	analysisChildVector.clear();
	return 0;
}

int AnalysisParent::runAnalysisThread()
{
	isRunning.store(true, std::memory_order_relaxed);
	setup();
	//Sleep(1000);
	while (isRunning.load(std::memory_order_relaxed)) {
		loop();
	}
	return 0;
}

int AnalysisParent::setup()
{
	int returnCode = SUCCESS;
	analysisChildVector.push_back(new AnalysisChild);
	returnCode = analysisChildVector.begin().operator*()->init(
		boxCache,
		carCache,
		&analysisChildrenUpdateQueue,
		carPool,
		&analysisFinishedCounterMutex,
		&analysisFinishedCounterInt,
		&analysisStepMutex,
		&analysisStepConditionVariable,
		&analysisStepInt
	);
	if (returnCode) {
		return returnCode;
	}
	returnCode = analysisChildVector.begin().operator*()->start();
	return returnCode;
}

int AnalysisParent::loop()
{
	analysisFinishedCounterInt.store(0, std::memory_order_relaxed);

	// acquire read lock on cache
	//wxLogDebug("Analysis is trying to get read lock");
	std::shared_lock<std::shared_mutex> toLock;
	carCache->acquireReadLock(&toLock);
	//wxLogDebug("Analysis got read lock");


	// notify all
	std::unique_lock<std::mutex> analysisStepLock(analysisStepMutex);
	analysisStepInt.store(true, std::memory_order_relaxed);
	analysisStepLock.unlock();
	analysisStepConditionVariable.notify_all();
	// while counter is less than length of children vector
	while (analysisFinishedCounterInt.load(std::memory_order_relaxed) < (int)analysisChildVector.size()) {
		// aggregate stuff in output queues from children
		this->aggregate();
		//Sleep(100);
	}
	while (analysisChildrenUpdateQueue.size()) {
		this->aggregate();// finish aggregating
	}

	if (analysisAggregationSet.empty()) {
		carCache->releaseReadLock(&toLock);
		//analysisStepLock.lock();
		analysisStepInt.store(false, std::memory_order_relaxed);
		//analysisStepLock.unlock();
		Sleep(ANALYSIS_PARENT_DELAY_MS);
		return 0;
	}

	for (CarData* currentCarDataPtr : analysisAggregationSet) {
		CarData* tempCarDataMergedPtr;
		carPool->getCar(&tempCarDataMergedPtr);						// Allocate a CarData 
		sharedCache<CarData*>::cacheIter tempCarDataOriginalIter;
		carCache->find(currentCarDataPtr, &tempCarDataOriginalIter);// grab iterators to originals
		(*tempCarDataMergedPtr) += *(*tempCarDataOriginalIter);		// copy original to update slot
		(*tempCarDataMergedPtr) += (*currentCarDataPtr);			// apply updates to copies
		//analysisUpdateQueue.push_back(tempCarDataMergedPtr);
		updateQueue->send(tempCarDataMergedPtr);					// Send stuff back to cache
		tempCarDataMergedPtr->printCar();	// For debugging :)
		
		// TODO: add second queue for sending stuff back to database
		// push updated copies into databases and cache update queues
		
	}
	analysisAggregationSet.clear();

	// TODO: Does this need to happen earlier?
	//analysisStepLock.lock();
	analysisStepInt.store(false, std::memory_order_relaxed);
	//analysisStepLock.unlock();
	// release read lock on cache
	carCache->releaseReadLock(&toLock);
	// try acquire write lock on cache
	wxLogDebug("Analysis attempting Cache write");
	carCache->updateCache();
	wxLogDebug("Analysis wrote cache");

	Sleep(100);

	return 0;
}

int AnalysisParent::aggregate()
{
	CarData * tmpCarDataPtr = nullptr; // Create temporary Car Data Pointer
	analysisChildrenUpdateQueue.pop(&tmpCarDataPtr); // Pop item from analysisChildrenUpdateQueue
	if (tmpCarDataPtr == nullptr) {
		return ERR_NULLPTR;
	}
	std::pair<std::set<CarData*, carTimeStampCompareLess>::iterator, bool> tmpPair = analysisAggregationSet.insert(tmpCarDataPtr);// TODO: add typedef for the iterator... seriously
	if (!(tmpPair.second)) {
		*(*(tmpPair.first)) += (*tmpCarDataPtr);	// Insert Item into analysisAggregationQueue or Merge into existing update entry
		// TODO: release the memory associated with this car in a less terrifying way
		delete tmpCarDataPtr;
	}
	// TODO: get rid of the memory leak (it should be fixed, but leaving this here until we know for sure)
	return 0;
}

