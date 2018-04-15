#pragma once

#include "Analysis.h"
#include "CacheBank.h"
#include "CarData.h"
#include "CarPool.h"
#include "DataInterface.h"
#include "DatabaseConnector.h"
#include "InterThreadComm.h"
#include "sharedCache.h"

//TODO: send analyzed data back to datastorage
class MIVCOTS
{
public:
	MIVCOTS();
	~MIVCOTS();

	typedef sharedCache<CarData*> mCache;

	int initialize();
	int initSerial(long baud, std::string port);
	// Start and stop the data interface, database, and analysis threads
	int start();
	int startSerial();
	int stop();

	// Read functions for the GUI
	int readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, long carNum);
	int readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, long carNum, unsigned int length);
	int endCacheRead(long carNum);

	bool newData(long carNum);

private:
	DataInterface dataSource;
	DatabaseConnector dataStorage;
	CacheBank cacheBank;
	AnalysisParent analysisEngine;
	CarPool carSource;

	InterThreadComm<CarData*, CarData*> dataSource_dataStorage;
	InterThreadComm<CarData*, CarData*> boxDataSource_dataStorage;
	InterThreadComm<CarData*, CarData*> analysis_dataStorage;
};

