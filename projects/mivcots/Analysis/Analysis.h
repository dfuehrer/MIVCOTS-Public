#pragma once


#include <vector>
#include <string>
#include "InterThreadComm/endpoint.h"
#include "InterThreadComm/sharedCache.h"
#include "CarPool/CarPool.h"
#include "CarData/CarData.h"
#include "AnalysisChild.h"
#include <wx/wx.h>
#include <fstream>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>


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

	// These are used to keep track of how many threads have finished processing
	// for this step
	std::mutex analysisFinishedCounterMutex;
	int analysisFinishedCounterInt;




	std::vector<AnalysisChild *> analysisChildVector;
	
	std::thread analysisParentThread; // Should be started when start() is called
	std::atomic<bool> isRunning;
	int runAnalysisThread();
	int setup();
	int loop();


};