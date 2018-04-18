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

	int initialize(CarPool* _carSource, std::string _cfgFileName, unsigned int _maxCacheSize);

	bool isNewCarNum(long carNum);
	int addCarNum(long carNum);
	int feed(CarData* toFeed);

	// Read cache functions
	typedef sharedCache<CarData*> mCache;
	int readCache(long carNum, mCache::cacheIter* startIter, mCache::cacheIter* endIter);
	int readCache(long carNum, mCache::cacheIter* startIter, mCache::cacheIter* endIter, unsigned int length);
	int releaseReadLock(long carNum);
	bool newRawData(long carNum);
	bool newAnalyzedData(long carNum);

	// Start/stop analyses
	int startAnalyses();
	int stopAnalyses();
	
	int getCarNums(std::vector<long>* toWrite);

private:
	std::map<int, carModule*> carModuleMap;
	std::mutex cmmMtx;
	std::string cfgFileName;
	CarPool* carSource;
	unsigned int maxCacheSize;

	bool isStarted;
	typedef std::map<int, carModule*>::iterator CMMiter;
	bool isNewCarNum(long carNum, CMMiter* loc);
	int addCarNum(long carNum, CMMiter* loc);
};