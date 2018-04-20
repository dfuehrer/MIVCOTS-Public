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
	rc |= cacheBank.initialize(&carSource, "", 40);
	return rc;
}

int MIVCOTS::initSerial(long baud, std::string port) {
	return dataSource.initialize(port, baud, dataSource_dataStorage.getEndpoint1(), &carSource);
}

int MIVCOTS::start()
{
	int rc = SUCCESS;
	rc |= dataStorage.start();
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

int MIVCOTS::acquireReadLock(long carNum)
{
	return cacheBank.acquireReadLock(carNum);
}

int MIVCOTS::readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, long carNum)
{
	return cacheBank.readCache(carNum, startIter, endIter);
}

int MIVCOTS::readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, long carNum, unsigned int length)
{
	return cacheBank.readCache(carNum, startIter, endIter, length);
}

int MIVCOTS::endCacheRead(long carNum)
{
	return cacheBank.releaseReadLock(carNum);
}

bool MIVCOTS::newData(long carNum)
{
	return cacheBank.newRawData(carNum) || cacheBank.newAnalyzedData(carNum);
}

int MIVCOTS::getCarNums(std::vector<long>* toWrite)
{
	return cacheBank.getCarNums(toWrite);
}

