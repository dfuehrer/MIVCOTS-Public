#ifndef SHARED_CACHE_H
#define SHARED_CACHE_H

#include <atomic>
#include <shared_mutex>
#include <deque>

#include "endpoint.h"
#include "error_codes.h"
#include "CarData.h"
#include "CarPool.h"
#include "key_defines.h"

// TODO: use a condition variable to give priority to updates
// TODO: actually write newdata functions
// TODO: rewrite update tracking logic to be less cancerous

template <class T> class sharedCache
{
public:

	typedef typename std::deque<T>::iterator cacheIter;

	sharedCache();
	~sharedCache();

	int initialize(unsigned int _maxSize,
				   endpoint<T>* _feedQ,
				   endpoint<T>* _updateQ,
				   CarPool* _carSource
				  );

	int feedCache();
	int updateCache();

	// Start cache read
	int acquireReadLock(std::shared_lock<std::shared_mutex> *toLock);

	// startIter points to the oldest element
	int readCache(cacheIter* startIter, cacheIter* endIter);
	int readCache(cacheIter* startIter, cacheIter* endIter, unsigned int length);
	int readCacheUpdates(cacheIter* startIter, cacheIter* endIter, unsigned int updateCount);
	int readLatestUpdate(cacheIter* iter, unsigned int updateCount);	// TODO: change to unsigned long
	int readLatestUpdateGreaterThan(cacheIter* iter, unsigned int minUpdateCount);	// TODO: change to unsigned long

	//int readData()
	int releaseReadLock(std::shared_lock<std::shared_mutex>* toUnlock);

	bool newRawData();
	bool newAnalyzedData();

	int find(CarData * toFind, cacheIter * iter);
	int find(CarData * toFind, int* ind);
	

private:
	unsigned int maxSize;
	endpoint<T>* feedQ;
	endpoint<T>* updateQ;
	CarPool* carSource;

	std::deque<T> buffer;
	mutable std::shared_mutex smtx;
	int findItem(T toFind, int* ind);
	
	int trackUpdates(T update);
	std::vector<T> latestUpdated;
};
#endif //SHARED_CACHE_H
