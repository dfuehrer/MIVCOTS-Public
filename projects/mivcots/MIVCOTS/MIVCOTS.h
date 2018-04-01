#pragma once

#include "Analysis.h"
#include "CarData.h"
#include "CarPool.h"
#include "DataInterface.h"
#include "DatabaseConnector.h"
#include "InterThreadComm.h"
#include "sharedCache.h"

class MIVCOTS
{
public:
	MIVCOTS();
	~MIVCOTS();

	typedef sharedCache<CarData*> mCache;

	int initialize();

	// Start and stop the data interface, database, and analysis threads
	int start();
	int stop();

	// Read functions for the GUI
	int readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter);
	int readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, unsigned int length);
	int endCacheRead();

private:
	DataInterface dataSource;
	DatabaseConnector dataStorage;
	sharedCache<CarData*> cache;
	Analysis analysisEngine;
	CarPool carSource;

	InterThreadComm<CarData*, CarData*> dataSource_dataStorage;
	InterThreadComm<CarData*, CarData*> dataStorage_cache;
	InterThreadComm<CarData*, CarData*> analysis_dataStorage;
	InterThreadComm<CarData*, CarData*> analysis_cache;
};

