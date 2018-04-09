#pragma once


#include <vector>
#include <string>
#include "InterThreadComm/endpoint.h"
#include "InterThreadComm/sharedCache.h"
#include "CarPool/CarPool.h"
#include "CarData/CarData.h"
#include <wx/wx.h>
#include <fstream>


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


};

