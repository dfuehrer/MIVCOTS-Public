#include "AnalysisChild.h"
#include "AnalysisReturnCodes.h"
#include <wx/log.h>


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
	CarPool * carPool
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
	
	return returnCode;
}

int AnalysisChild::start()
{
	return 0;
}

int AnalysisChild::stop()
{
	isRunning = false;
	analysisThread.join();
	return 0;
}

int AnalysisChild::runThread()
{
	isRunning = true;
	while (isRunning) {	// Keep going as long as the thread is alive
		
	}
	return 0;
}


