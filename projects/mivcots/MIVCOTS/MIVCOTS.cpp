#include "MIVCOTS.h"

MIVCOTS::MIVCOTS()
{
}

MIVCOTS::~MIVCOTS()
{
}

int MIVCOTS::initialize()
{
	return 0;
}

int MIVCOTS::start()
{
	return 0;
}

int MIVCOTS::stop()
{
	return 0;
}

int MIVCOTS::readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter)
{
	return cache.readCache(startIter, endIter);
}

int MIVCOTS::readCache(mCache::cacheIter* startIter, mCache::cacheIter* endIter, unsigned int length)
{
	return cache.readCache(startIter, endIter, length);
}

int MIVCOTS::endCacheRead()
{
	return cache.releaseReadLock();
}

