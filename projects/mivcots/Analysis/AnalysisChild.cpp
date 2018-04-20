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
	endpoint<CarData*> * updateQueue, 
	CarPool * carPool,
	std::mutex * analysisFinishedCounterMutex,
	std::atomic<int> * analysisFinishedCounterInt,
	std::mutex * analysisStepMutex,
	std::condition_variable * analysisStepConditionVariable
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
	
	return returnCode;
}

int AnalysisChild::start()
{
	return 0;
}

int AnalysisChild::stop()
{
	isRunning.store(false, std::memory_order_relaxed);
	// some other stuff needs to happen here
	analysisThread.join();
	return 0;
}

int AnalysisChild::runThread()
{
	isRunning.store(true,std::memory_order_relaxed);// TODO: move this outside of the thread
	this->setup();
	while (isRunning.load(std::memory_order_relaxed)) {	// Keep going as long as the thread is alive
		
		// wait to be notified
		std::unique_lock<std::mutex> analysisStepLock(*analysisStepMutex);
		analysisStepConditionVariable->wait(analysisStepLock);// TODO: add loop
		analysisStepLock.unlock();
		// TODO: Check for spurious wakeup


		// do things
		this->loop();

		// increment semaphore
		std::unique_lock<std::mutex> analysisFinishedCounterLock(*analysisFinishedCounterMutex);	// Create lock and block until mutex is locked
		analysisFinishedCounterInt++;	// Increment Semaphore
		analysisFinishedCounterLock.unlock();	// Unlock mutex 

	}
	return 0;
}

int AnalysisChild::setup()
{
	return 0;
}
#define ANALYSIS_COUNT "ZZ"	// TODO: Move this to defines file
int AnalysisChild::loop()
{
	
	
	sharedCache<CarData *>::cacheIter startIter, endIter, tempIter;
	carCache->readCache(&startIter, &endIter);
	for (tempIter = startIter; tempIter != endIter; tempIter++) {
		unsigned long analysisCount;
		(*tempIter)->get(ANALYSIS_COUNT, &analysisCount);
		if (analysisCount == 0) {
			// get new CarData
			CarData * tempCarDataPtr;
			carPool->getCar(&tempCarDataPtr);
			// update analysis count
			tempCarDataPtr->set(std::string(ANALYSIS_COUNT), (unsigned long)1);
			tempCarDataPtr->addKey(std::string(ANALYSIS_COUNT));
			// do analysis
			long latRaw = 0, lonRaw = 0, angleRaw = 0;
			(*tempIter)->get(std::string(LAT), &latRaw);
			(*tempIter)->get(std::string(LON), &lonRaw);
			(*tempIter)->get(std::string(HEADING), &angleRaw);
			tempCarDataPtr->addKey(std::string(LAT) + std::string("D"));
			tempCarDataPtr->addKey(std::string(LON) + std::string("D"));
			tempCarDataPtr->addKey(std::string(HEADING) + std::string("D"));
			tempCarDataPtr->set(std::string(LAT) + std::string("D"), (double)latRaw / 1000000.0);
			tempCarDataPtr->set(std::string(LON) + std::string("D"), (double)lonRaw / 1000000.0);
			tempCarDataPtr->set(std::string(HEADING) + std::string("D"), (double)angleRaw / 100.0);

			// push to update Queue
			updateQueue->send(tempCarDataPtr);
			
			
		}
	}
	return 0;
}


