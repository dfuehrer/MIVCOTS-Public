#pragma once

#include "Analysis.h"
#include "CarData.h"
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

	int readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter);
	int readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, unsigned int length);
	int endCacheRead();

private:
	DataInterface dataSource;
	DatabaseConnector dataStorage;
	sharedCache<CarData*> cache;
	Analysis analysisEngine;

	InterThreadComm<CarData*, CarData*> dataSource_dataStorage;
	InterThreadComm<CarData*, CarData*> dataStorage_cache;
	InterThreadComm<CarData*, CarData*> analysis_dataStorage;
	InterThreadComm<CarData*, CarData*> analysis_cache;
};

