#include "MIVCOTS.h"

MIVCOTS::MIVCOTS()
{
}

MIVCOTS::~MIVCOTS()
{
}

int MIVCOTS::initialize()
{
	int rc = SUCCESS;
	rc |= dataStorage.initialize(dataSource_dataStorage.getEndpoint2(), 
		boxDataSource_dataStorage.getEndpoint2(), &carSource, &cacheBank);
	rc |= cacheBank.initialize(&carSource, "", 4000);
	return rc;
}

int MIVCOTS::initSerial(long baud, std::string port) {
	return dataSource.initialize(port, baud, dataSource_dataStorage.getEndpoint1(), &carSource);
}

int MIVCOTS::start()
{
	int rc = SUCCESS;
	rc |= dataStorage.start();
	cacheBank.startAnalyses();
	return rc;
}

int MIVCOTS::startSerial() {
	return dataSource.start();
}

bool MIVCOTS::serialOpen()
{
	return dataSource.isSerialRunning();
}

int MIVCOTS::stopSerial()
{
	return dataSource.stop();
}

int MIVCOTS::stop()
{
	return 0;
}

int MIVCOTS::acquireReadLock(long carNum, std::shared_lock<std::shared_mutex>* toLock)
{
	return cacheBank.acquireReadLock(carNum, toLock);
}

int MIVCOTS::readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, long carNum)
{
	return cacheBank.readCache(carNum, startIter, endIter);
}

int MIVCOTS::readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, long carNum, unsigned int length)
{
	return cacheBank.readCache(carNum, startIter, endIter, length);
}

int MIVCOTS::readCacheUpdates(long carNum, mCache::cacheIter * startIter, mCache::cacheIter * endIter, unsigned int updateCount)
{
	return cacheBank.readCacheUpdates(carNum, startIter, endIter, updateCount);
}

int MIVCOTS::readLatestUpdate(long carNum, mCache::cacheIter * iter, unsigned long updateCount)
{
	return cacheBank.readLatestUpdate(carNum, iter, updateCount);
}

int MIVCOTS::endCacheRead(long carNum, std::shared_lock<std::shared_mutex>* toLock)
{
	return cacheBank.releaseReadLock(carNum, toLock);
}

bool MIVCOTS::newData(long carNum)
{
	return cacheBank.newRawData(carNum) || cacheBank.newAnalyzedData(carNum);
}

int MIVCOTS::getCarNums(std::vector<long>* toWrite)
{
	return cacheBank.getCarNums(toWrite);
}

int MIVCOTS::startPlayback(databaseInfo playbackRequest, double timeFactor)
{
	return dataStorage.startPlayback(playbackRequest, timeFactor);
}

