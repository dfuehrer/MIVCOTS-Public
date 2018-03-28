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
	return SUCCESS;
}

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

int sharedCache<CarData*>::readCache()
{
	std::shared_lock<std::shared_mutex> slock(smtx);
	slock.lock();

	slock.unlock();
	return 0;
}
