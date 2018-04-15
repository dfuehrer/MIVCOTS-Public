#include "CacheBank.h"

CacheBank::CacheBank()
{
	// Add a car module but don't start the analysis for car 0 (the box)
	carModule* toInsert = new carModule;
	carModuleMap[0] = toInsert;

	toInsert->cache.initialize(maxCacheSize,
		toInsert->inputQ.getEndpoint2(),
		toInsert->updateQ.getEndpoint2());
}

CacheBank::~CacheBank()
{
	CMMiter iter;
	for (iter = carModuleMap.begin(); iter != carModuleMap.end(); ++iter) {
		if (iter->second != nullptr) {
			delete iter->second;
		}
	}

	carModuleMap.clear();
}

int CacheBank::initialize(CarPool * _carSource, std::string _cfgFileName, unsigned int _maxCacheSize)
{
	if (_carSource == nullptr) {
		return NULLPTRERR;
	}
	
	carSource = _carSource;
	cfgFileName = _cfgFileName;
	maxCacheSize = _maxCacheSize;

	return SUCCESS;
}

bool CacheBank::isNewCarNum(long carNum)
{
	CMMiter loc;
	return isNewCarNum(carNum, &loc);
}

int CacheBank::addCarNum(long carNum)
{
	CMMiter loc;
	return addCarNum(carNum, &loc);
}

int CacheBank::feed(CarData* toFeed)
{
	if (toFeed == nullptr) {
		return NULLPTRERR;
	}
	int rc = SUCCESS;
	long carNum;
	rc |= toFeed->get("ID", &carNum);

	wxLogDebug("Inserting data for car number %d", carNum);

	CMMiter loc;
	int rc2 = addCarNum(carNum, &loc);
	if (rc2 != ELEMENTEXISTS) {
		rc |= rc2;
	}
	rc |= loc->second->cache.feedCache();
	return  rc;
	//return 0;
}

int CacheBank::readCache(long carNum, mCache::cacheIter * startIter, mCache::cacheIter * endIter)
{
	return carModuleMap.at(carNum)->cache.readCache(startIter, endIter);
}

int CacheBank::readCache(long carNum, mCache::cacheIter * startIter, mCache::cacheIter * endIter, unsigned int length)
{
	return carModuleMap.at(carNum)->cache.readCache(startIter, endIter, length);
}

int CacheBank::startAnalyses()
{
	CMMiter iter;
	int rc = SUCCESS;
	for (iter = carModuleMap.begin(); iter != carModuleMap.end(); ++iter) {
		rc |= iter->second->analysis.start();
	}

	return rc;
}

int CacheBank::stopAnalyses()
{
	CMMiter iter;
	int rc = SUCCESS;
	for (iter = carModuleMap.begin(); iter != carModuleMap.end(); ++iter) {
		rc |= iter->second->analysis.stop();
	}

	return rc;
}

bool CacheBank::isNewCarNum(long carNum, CMMiter* loc)
{
	*loc = carModuleMap.find(carNum);
	if (*loc == carModuleMap.end()) {
		return true;
	}
	else {
		return false;
	}
}

int CacheBank::addCarNum(long carNum, CMMiter* loc)
{
	if (loc == nullptr) {
		return NULLPTRERR;
	}

	if (isNewCarNum(carNum, loc)) {
		carModule* toInsert = new carModule;
		carModuleMap[carNum] = toInsert;

		int rc = SUCCESS;
		rc |= toInsert->cache.initialize(maxCacheSize,
			toInsert->inputQ.getEndpoint2(),
			toInsert->updateQ.getEndpoint2());

		rc |= toInsert->analysis.init(&(carModuleMap.at(0)->cache),
			&(toInsert->cache), toInsert->updateQ.getEndpoint1(),
			carSource, cfgFileName);

		if (isStarted) {
			rc |= toInsert->analysis.start();
		}
		return rc;
	}
	else {
		return ELEMENTEXISTS;
	}
}
