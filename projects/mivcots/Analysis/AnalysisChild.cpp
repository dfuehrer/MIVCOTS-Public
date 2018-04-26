#include "AnalysisChild.h"
#include "AnalysisReturnCodes.h"
#include <wx/log.h>
#include "key_defines.h"



AnalysisChild::AnalysisChild()
{
}


AnalysisChild::~AnalysisChild()
{
}

int AnalysisChild::init(
	sharedCache<CarData*> * boxCache,
	sharedCache<CarData*> * carCache,
	lockedQueue<CarData*> * updateQueue,
	CarPool * carPool,
	std::mutex * analysisFinishedCounterMutex,
	std::atomic<int> * analysisFinishedCounterInt,
	std::mutex * analysisStepMutex,
	std::condition_variable * analysisStepConditionVariable,
	std::atomic<bool>* analysisStepInt
)
{
	int returnCode = 0;
	if (boxCache != nullptr) {
		this->boxCache = boxCache;
	}
	else {
		wxLogError("Analysis Child - Init Function: boxCache Pointer is Null");
		returnCode |= ERR_BOX_CACHE_PTR_IS_NULL;
	}
	if (carCache != nullptr) {
		this->carCache = carCache;
	}
	else {
		wxLogError("Analysis Child - Init Function: carCache Pointer is Null");
		returnCode |= ERR_CAR_CACHE_PTR_IS_NULL;
		return returnCode;
	}
	if (updateQueue != nullptr) {
		this->updateQueue = updateQueue;
	}
	else {
		wxLogError("Analysis Child - Init Function: updateQueue Pointer is Null");
		returnCode |= ERR_UPDATE_QUEUE_PTR_IS_NULL;
		return returnCode;
	}
	if (carPool != nullptr) {
		this->carPool = carPool;
	}
	else {
		wxLogError("Analysis Child - Init Function: carPool Pointer is Null");
		returnCode |= ERR_CAR_CACHE_PTR_IS_NULL;
		return returnCode;
	}
	// TODO: Add error checking here
	this->analysisFinishedCounterMutex = analysisFinishedCounterMutex;
	this->analysisFinishedCounterInt = analysisFinishedCounterInt;
	this->analysisStepMutex = analysisStepMutex;
	this->analysisStepConditionVariable = analysisStepConditionVariable;
	this->analysisStepInt = analysisStepInt;
	return returnCode;
}

int AnalysisChild::start()
{
	this->analysisThread = std::thread(&AnalysisChild::runThread, this);
	return 0;
}

int AnalysisChild::stop()
{

	isRunning.store(false, std::memory_order_relaxed);
	// some other stuff needs to happen here
	if (analysisThread.joinable()) {
		analysisThread.join();
	}
	
	return 0;
}

int AnalysisChild::runThread()
{
	isRunning.store(true, std::memory_order_relaxed);// TODO: move this outside of the thread
	this->setup();
	while (isRunning.load(std::memory_order_relaxed)) {	// Keep going as long as the thread is alive

		// wait to be notified
		std::unique_lock<std::mutex> analysisStepLock(*analysisStepMutex);
		do {
			analysisStepConditionVariable->wait(analysisStepLock);// TODO: add loop
		} while (!(analysisStepInt->load(std::memory_order_relaxed)));

		analysisStepLock.unlock();
		// TODO: Check for spurious wakeup



		// do things
		this->loop();

		// increment semaphore
		std::unique_lock<std::mutex> analysisFinishedCounterLock(*analysisFinishedCounterMutex);	// Create lock and block until mutex is locked
		analysisFinishedCounterInt->store(analysisFinishedCounterInt->load(std::memory_order_relaxed) + 1,
			std::memory_order_relaxed);	// Increment Semaphore
		analysisFinishedCounterLock.unlock();	// Unlock mutex 

	}
	return 0;
}

int AnalysisChild::setup()
{
	return 0;
}
//#define ANALYSIS_COUNT "ZZ"	// TODO: Move this to defines file
int AnalysisChild::loop()
{

	int returnCode = SUCCESS;
	//wxLogDebug("Analysis Child Loop");
	sharedCache<CarData *>::cacheIter startIter, endIter, tempIter;
	carCache->readCache(&startIter, &endIter);
	for (tempIter = startIter; tempIter != endIter; tempIter++) {
		unsigned long analysisCount;
		(*tempIter)->get(ANALYSIS_COUNT_U, &analysisCount);
		if (analysisCount == 0) {
			// get new CarData
			CarData * tempCarDataPtr = nullptr;
			carPool->getCar(&tempCarDataPtr);
			if (tempCarDataPtr == nullptr) {
				returnCode = ERR_ANALYSIS_CHILD | ERR_LOOP | ERR_NULLPTR;
			}
			// update analysis count
			tempCarDataPtr->addKey(ANALYSIS_COUNT_U);
			tempCarDataPtr->set(ANALYSIS_COUNT_U, (unsigned long)1);

			// do analysis
			long latRaw = 0, lonRaw = 0, angleRaw = 0, timeStamp = 0, mphRaw = 0;
			(*tempIter)->get(TIME_S, &timeStamp);
			(*tempIter)->get(LAT_S, &latRaw);
			(*tempIter)->get(LON_S, &lonRaw);
			(*tempIter)->get(HEADING_S, &angleRaw);
			(*tempIter)->get(MPH_S, &mphRaw);

			tempCarDataPtr->addKey(TIME_S);
			tempCarDataPtr->addKey(LAT_D);
			tempCarDataPtr->addKey(LON_D);
			tempCarDataPtr->addKey(HEADING_D);
			tempCarDataPtr->addKey(MPH_D);

			tempCarDataPtr->set(TIME_S, (long)timeStamp);
			tempCarDataPtr->set(LAT_D, (double)latRaw / 1000000.0);
			tempCarDataPtr->set(LON_D, (double)lonRaw / 1000000.0);
			tempCarDataPtr->set(HEADING_D, (double)angleRaw / 100.0);
			tempCarDataPtr->set(MPH_D, (double)mphRaw / 10.0);


			// push to update Queue
			updateQueue->push(tempCarDataPtr);


		}
	}
	return 0;
}


