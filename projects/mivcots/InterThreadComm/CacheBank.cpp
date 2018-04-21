#include "CacheBank.h"

CacheBank::CacheBank()
{

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
		return ERR_NULLPTR;
	}
	
	carSource = _carSource;
	cfgFileName = _cfgFileName;
	maxCacheSize = _maxCacheSize;

	// Add a car module but don't start the analysis for car 0 (the box)
	carModule* toInsert = new carModule;
	carModuleMap[0] = toInsert;

	toInsert->cache.initialize(maxCacheSize,
		toInsert->inputQ.getEndpoint2(),
		toInsert->updateQ.getEndpoint2());

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
	//std::lock_guard<std::mutex> lock(cmmMtx);
	if (toFeed == nullptr) {
		return ERR_NULLPTR;
	}
	int rc = SUCCESS;
	long carNum;
	rc |= toFeed->get("AAS", &carNum);

	wxLogDebug("Inserting data for car number %d", carNum);

	CMMiter loc;
	int rc2 = addCarNum(carNum, &loc);
	if (rc2 != ERR_ELEMENTEXISTS) {
		rc |= rc2;
	}
	rc |= loc->second->inputQ.getEndpoint1()->send(toFeed);
	rc |= loc->second->cache.feedCache();
	return  rc;
	//return 0;
}

int CacheBank::acquireReadLock(long carNum)
{
	if (isNewCarNum(carNum)) {
		return ERR_NOTFOUND;
	}
	return carModuleMap.at(carNum)->cache.acquireReadLock();
}

int CacheBank::readCache(long carNum, mCache::cacheIter * startIter, mCache::cacheIter * endIter)
{
	//std::lock_guard<std::mutex> lock(cmmMtx);
	if (isNewCarNum(carNum)) {
		return ERR_NOTFOUND;
	}
	return carModuleMap.at(carNum)->cache.readCache(startIter, endIter);
}

int CacheBank::readCache(long carNum, mCache::cacheIter * startIter, mCache::cacheIter * endIter, unsigned int length)
{
	//std::lock_guard<std::mutex> lock(cmmMtx);
	if (isNewCarNum(carNum)) {
		return ERR_NOTFOUND;
	}
	return carModuleMap.at(carNum)->cache.readCache(startIter, endIter, length);
}

int CacheBank::releaseReadLock(long carNum)
{
	//std::lock_guard<std::mutex> lock(cmmMtx);
	return carModuleMap.at(carNum)->cache.releaseReadLock();
}

bool CacheBank::newRawData(long carNum)
{
	//std::lock_guard<std::mutex> lock(cmmMtx);
	return carModuleMap.at(carNum)->cache.newRawData();
}

bool CacheBank::newAnalyzedData(long carNum)
{
	return carModuleMap.at(carNum)->cache.newAnalyzedData();
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

int CacheBank::getCarNums(std::vector<long>* toWrite)
{
	if (toWrite == nullptr) {
		return ERR_NULLPTR;
	}

	toWrite->clear();

	CMMiter iter;
	for (iter = carModuleMap.begin(); iter != carModuleMap.end(); ++iter) {
		toWrite->push_back(iter->first);
	}

	return SUCCESS;
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
		return ERR_NULLPTR;
	}

	if (isNewCarNum(carNum, loc)) {
		carModule* toInsert = new carModule;
		carModuleMap[carNum] = toInsert;

		int rc = SUCCESS;
		rc |= toInsert->cache.initialize(maxCacheSize,
			toInsert->inputQ.getEndpoint2(),
			toInsert->updateQ.getEndpoint2());

		toInsert->analysis.init(&(carModuleMap.at(0)->cache),
			&(toInsert->cache), toInsert->updateQ.getEndpoint1(),
			carSource, cfgFileName);

		if (isStarted) {
			rc |= toInsert->analysis.start();
		}

		*loc = carModuleMap.find(carNum);
		return rc;
	}
	else {
		return ERR_ELEMENTEXISTS;
	}
}
