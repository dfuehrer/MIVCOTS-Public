#include "Analysis.h"
#include <wx/log.h>
#include "AnalysisReturnCodes.h"



AnalysisParent::AnalysisParent()
{
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
		updateQueue,
		carPool,
		&analysisFinishedCounterMutex,
		&analysisFinishedCounterInt,
		&analysisStepMutex,
		&analysisStepConditionVariable
	);
	if (returnCode) {
		return returnCode;
	}
	returnCode = analysisChildVector.begin().operator*()->start();
	return returnCode;
}

int AnalysisParent::loop()
{
	// acquire read lock on cache
	carCache->acquireReadLock();
	// notify all
	std::unique_lock<std::mutex> analysisStepLock(analysisStepMutex);
	analysisStepInt = true;
	analysisStepConditionVariable.notify_all();
	// while counter is less than length of children vector
	while (analysisFinishedCounterInt.load(std::memory_order_relaxed) < analysisChildVector.size()) {
		// aggregate stuff in output queues from children
		this->aggregate();
	}
	while (analysisChildrenUpdateQueue.size()) {
		this->aggregate();// finish aggregating
	}

	for (CarData* currentCarDataPtr : analysisAggregationSet) {
		CarData* tempCarDataPtr;
		carPool->getCar(&tempCarDataPtr);

		//analysisUpdateQueue
	}


	// grab copies of originals
	// apply updates to copies
	// push updated copies into databases and cache update queues
	// release read lock on cache
	// try acquire write lock on cache

	return 0;
}

int AnalysisParent::aggregate()
{
	CarData * tmpCarDataPtr; // Create temporary Car Data Pointer
	analysisChildrenUpdateQueue.pop(&tmpCarDataPtr); // Pop item from analysisChildrenUpdateQueue
	if (tmpCarDataPtr == nullptr) {
		return NULLPTRERR;
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

//int Analysis::ControlChart(int GraphPoints) {
//	int i = 0;
//	double DataPointSum = 0;
//	double StandDeviation = 0;
//	double TopEqn = 0;
//	double UpperControlLimit = 0;
//	double LowerControlLimit = 0;
//	double AvgData = 0;
//	double Warning = 0;
//	//begin Initializaatshion
//	for (i = 0; i < GraphPoints - 1; i++) {
//		DataPointSum = DataPoints[i] + DataPointSum;
//	}
//	AvgData = DataPointSum / (GraphPoints - 1);
//	for (i = 0; i < GraphPoints - 1; i++) {
//		TopEqn = pow(DataPoints[i] - DataPointSum, 2)+ TopEqn;
//	}
//	StandDeviation = pow(TopEqn / (GraphPoints - 1), 0.5);
//	UpperControlLimit = DataPointSum + StandDeviation;
//	LowerControlLimit = DataPointSum - StandDeviation;
//
//	if (DataPoints[GraphPoints] >= UpperControlLimit) {
//		Warning = 1;
//	}
//	if (DataPoints[GraphPoints] <= LowerControlLimit) {
//		Warning = 1;
//	}
//	else
//		Warning = 0; 
//	return 0;
//}