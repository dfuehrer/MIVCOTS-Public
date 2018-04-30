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
	AnalysisSyncVars * analysisSyncVars
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
	this->analysisSyncVars = analysisSyncVars;
	return returnCode;
}

int AnalysisChild::start()
{
	this->analysisThread = std::thread(&AnalysisChild::runThread, this);
	return SUCCESS;
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
		std::unique_lock<std::mutex> analysisStepLock(*(analysisSyncVars->analysisStepMutex));
		do {
			analysisSyncVars->analysisStepConditionVariable->wait(analysisStepLock);// TODO: add loop
		} while (!(analysisSyncVars->analysisStepInt->load(std::memory_order_relaxed)));

		analysisStepLock.unlock();
		// TODO: Run some kind of analysis to make sure the variables are set in the correct order.

		// If the program is trying to exit, let it
		if (!(isRunning.load(std::memory_order_relaxed))) {
			return 0;
		}


		// do things
		this->loop();

		// increment semaphore
		std::unique_lock<std::mutex> analysisFinishedCounterLock(*(this->analysisSyncVars->analysisFinishedCounterMutex));	// Create lock and block until mutex is locked
		this->analysisSyncVars->analysisFinishedCounterInt->store(this->analysisSyncVars->analysisFinishedCounterInt->load(std::memory_order_relaxed) + 1,
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
			long latRaw = 0, lonRaw = 0, angleRaw = 0, timeStamp = 0, mphRaw = 0, tempRaw = 0, tempAlt = 0, accelZ = 0;
			(*tempIter)->get(TIME_S, &timeStamp);
			(*tempIter)->get(LAT_S, &latRaw);
			(*tempIter)->get(LON_S, &lonRaw);
			(*tempIter)->get(HEADING_S, &angleRaw);
			(*tempIter)->get(MPH_S, &mphRaw);
			(*tempIter)->get(TEMP_S, &tempRaw);
			(*tempIter)->get(ACC_Z_S, &accelZ);
			//(*tempIter)->get(ALTITUDE_S, &tempAlt);

			tempCarDataPtr->addKey(TIME_S);
			tempCarDataPtr->addKey(LAT_D);
			tempCarDataPtr->addKey(LON_D);
			tempCarDataPtr->addKey(HEADING_D);
			tempCarDataPtr->addKey(MPH_D);
			tempCarDataPtr->addKey(TEMP_D);
			tempCarDataPtr->addKey(ACC_Z_D);
			//tempCarDataPtr->addKey(ALTITUDE_D);

			tempCarDataPtr->set(TIME_S, (long)timeStamp);
			tempCarDataPtr->set(LAT_D, (double)latRaw / 1000000.0);
			tempCarDataPtr->set(LON_D, (double)lonRaw / 1000000.0);
			tempCarDataPtr->set(HEADING_D, (double)angleRaw / 100.0);
			if (mphRaw != 255) { // GPS is wrong - this is a default value
				tempCarDataPtr->set(MPH_D, (double)mphRaw / 1.0);
			}
			
			// TODO: this is very specific to the imu and should be done on chip
			tempCarDataPtr->set(TEMP_D, ((double)tempRaw / 340.00 + 36.53) * 1.8 + 32);
			//tempCarDataPtr->set(ALTITUDE_D, (double)tempAlt);

			// TODO: need to know accel range before hand.
			double accelRange = 2.0;
			double accelZ_conv;
			if (accelZ != 0) {
				accelZ_conv = double(accelZ) / ((double)(1 << 16)) * accelRange * 2;
				//wxLogMessage("Z accel: %f", accelZ_conv);
			}
			
			tempCarDataPtr->set(ACC_Z_D, accelZ_conv);

			// push to update Queue
			updateQueue->push(tempCarDataPtr);


		}
	}
	return 0;
}


