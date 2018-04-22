#pragma once


#include <vector>
#include <string>
#include "InterThreadComm/endpoint.h"
#include "InterThreadComm/lockedQueue.h"
#include "InterThreadComm\sharedCache.h"
#include "CarPool/CarPool.h"
#include "CarData/CarData.h"
#include "AnalysisChild.h"
#include <wx/wx.h>
#include <fstream>
#include <set>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#define ANALYSIS_PARENT_DELAY_MS 1

class AnalysisParent
{
public:
	AnalysisParent();
	~AnalysisParent();
	/* 
		Initialize Parent Analysis Object:
			connect to data sources and sinks
			read config file
	*/
	int init(
		sharedCache<CarData *> * boxCache, 
		sharedCache<CarData *> * carCache, 
		endpoint<CarData *> * updateQueue, 
		CarPool * carPool, 
		std::string configFileName
	);
	
	// Spin up threads and stuff
	int start();
	// Join all analysis threads, perform cleanup, and exit
	int stop();


private:
	sharedCache<CarData*> * boxCache;
	sharedCache<CarData*> * carCache;
	endpoint<CarData*> * updateQueue;
	CarPool * carPool;
	std::string configFileName;
	std::ifstream configFileInputStream;

	// These are used to tell the analysis children when to start
	std::mutex analysisStepMutex;
	std::condition_variable analysisStepConditionVariable;
	std::atomic<bool> analysisStepInt;

	// These are used to keep track of how many threads have finished processing
	// for this step
	std::mutex analysisFinishedCounterMutex;
	std::atomic<int> analysisFinishedCounterInt;
	



	std::vector<AnalysisChild *> analysisChildVector; // Stores AnalysisChild and anything that inherits from it
	lockedQueue<CarData *> analysisChildrenUpdateQueue; // Queue for CarData pointers with transactional updates from AnalysisChildren
	std::set<CarData *,carTimeStampCompareLess> analysisAggregationSet;	// Stores Aggregated Transactional updates in CarData Pointers
	std::deque<CarData *> analysisUpdateQueue;		// Stores Copies of original data to be merged with transactional updates
	
	std::thread analysisParentThread; // Should be started when start() is called
	std::atomic<bool> isRunning;
	int runAnalysisThread();
	int setup();
	int loop();
	int aggregate();


};