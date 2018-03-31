#pragma once

#include <shared_mutex>
#include <deque>

#include "endpoint.h"
#include "error_codes.h"

// TODO: use a condition variable to give priority to updates

template <class T> class sharedCache
{
public:

	typedef typename std::deque<T>::iterator cacheIter;

	sharedCache();
	~sharedCache();

	int initialize(unsigned int _maxSize,
				   endpoint<T>* _feedQ,
				   endpoint<T>* _updateQ
				  );

	int feedCache();
	int updateCache();

	// startIter points to the oldest element
	int readCache(cacheIter* startIter, cacheIter* endIter);
	int readCache(cacheIter* startIter, cacheIter* endIter, unsigned int length);
	int releaseReadLock();

private:
	unsigned int maxSize;
	endpoint<T>* feedQ;
	endpoint<T>* updateQ;

	std::deque<T> buffer;
	std::shared_mutex smtx;
	std::shared_lock<std::shared_mutex>* slock;
	
	int findItem(T toFind, int* ind);
};
