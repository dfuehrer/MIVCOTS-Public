#pragma once

#include <map>
#include <mutex>
#include <wx/wx.h>

#include "Analysis.h"
#include "CarData.h"
#include "CarPool.h"
#include "InterThreadComm.h"
#include "sharedCache.h"

// TODO: thread safety without deadlocks
// minor TODO: make getCarNums() more efficient

typedef struct carModule_t {
	sharedCache<CarData*> cache;
	AnalysisParent analysis;
	InterThreadComm<CarData*> inputQ;
	InterThreadComm<CarData*> updateQ;
} carModule;

class CacheBank {
public:
	CacheBank();
	~CacheBank();

	int initialize(endpoint<CarData*>* _storageQueue, CarPool* _carSource, 
				   std::string _cfgFileName, unsigned int _maxCacheSize);

	bool isNewCarNum(long carNum);
	int addCarNum(long carNum);
	int feed(CarData* toFeed);

	// Read cache functions
	typedef sharedCache<CarData*> mCache;
	int acquireReadLock(long carNum, std::shared_lock<std::shared_mutex>* toLock);
	int readCache(long carNum, mCache::cacheIter* startIter, mCache::cacheIter* endIter);
	int readCache(long carNum, mCache::cacheIter* startIter, mCache::cacheIter* endIter, unsigned int length);
	int readCacheUpdates(long carNum, mCache::cacheIter* startIter, mCache::cacheIter* endIter, unsigned int updateCount);
	int readLatestUpdate(long carNum, mCache::cacheIter* iter, unsigned long updateCount);
	int releaseReadLock(long carNum, std::shared_lock<std::shared_mutex>* toLock);
	bool newRawData(long carNum);
	bool newAnalyzedData(long carNum);

	// Start/stop analyses
	int startAnalyses();
	int stopAnalyses();
	
	int getCarNums(std::vector<long>* toWrite, bool* isChanged);

private:
	std::map<int, carModule*> carModuleMap;
	std::mutex cmmMtx;

	endpoint<CarData*>* storageQueue;
	std::string cfgFileName;
	CarPool* carSource;
	unsigned int maxCacheSize;

	bool isStarted;
	typedef std::map<int, carModule*>::iterator CMMiter;
	bool isNewCarNum(long carNum, CMMiter* loc);
	int addCarNum(long carNum, CMMiter* loc);
};