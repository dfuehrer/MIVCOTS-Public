#include "sharedCache.h"

#include <mutex>
#include "CarData.h"

sharedCache<CarData*>::sharedCache()
{
}

sharedCache<CarData*>::~sharedCache()
{
}

int sharedCache<CarData*>::initialize(unsigned int _maxSize, endpoint<CarData*>* _feedQ, endpoint<CarData*>* _updateQ)
{
	maxSize = _maxSize;

	if ((feedQ == nullptr) ||
		(updateQ == nullptr)) {
		return NULLPTRERR;
	}

	feedQ = _feedQ;
	updateQ = _updateQ;

	slock = new std::shared_lock<std::shared_mutex>(smtx);

	if (slock == nullptr) {
		return MUTEXERR;
	}

	return SUCCESS;
}

// New data goes in the back of the deque
int sharedCache<CarData*>::feedCache()
{
	std::unique_lock<std::shared_mutex> ulock(smtx);
	ulock.lock();

	ulock.unlock();
	return 0;
}

int sharedCache<CarData*>::updateCache()
{
	std::unique_lock<std::shared_mutex> ulock(smtx);
	ulock.lock();

	ulock.unlock();
	return 0;
}

int sharedCache<CarData*>::readCache(cacheIter* startIter, cacheIter* endIter){
	if ((cacheIter == nullptr) || (endIter == nullptr)) {
		return NULLPTRERR;
	}

	slock->lock();

	*startIter = buffer.begin();
	*endIter = buffer.end();

	return SUCCESS;
}

int sharedCache<CarData*>::readCache(cacheIter* startIter, cacheIter* endIter, unsigned int length){
	if ((cacheIter == nullptr) || (endIter == nullptr)) {
		return NULLPTRERR;
	}

	if ((length > maxSize) || (length > buffer.size())) {
		return OUTOFRANGE;
	}

	slock->lock();

	*startIter = buffer.begin();
	*endIter = buffer.begin() + length;
}

int sharedCache<CarData*>::releaseReadLock(){
	slocl->unlock();
}