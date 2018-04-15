#pragma once

#include <map>
#include <wx/wx.h>

#include "Analysis.h"
#include "CarData.h"
#include "CarPool.h"
#include "InterThreadComm.h"
#include "sharedCache.h"

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
	
	// Start/stop analyses
	int startAnalyses();
	int stopAnalyses();

private:
	std::map<int, carModule*> carModuleMap;
	std::string cfgFileName;
	CarPool* carSource;
	unsigned int maxCacheSize;

	bool isStarted;
	typedef std::map<int, carModule*>::iterator CMMiter;
	bool isNewCarNum(long carNum, CMMiter* loc);
	int addCarNum(long carNum, CMMiter* loc);
};