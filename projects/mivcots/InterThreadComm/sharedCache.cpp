#include "sharedCache.h"

#include <mutex>
#include "CarData.h"

sharedCache<CarData*>::sharedCache()
{
}

sharedCache<CarData*>::~sharedCache()
{
	if (slock != nullptr) {
		delete slock;
	}
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

	slock = new std::shared_lock<std::shared_mutex>(smtx, std::defer_lock);

	if (slock == nullptr) {
		return MUTEXERR;
	}

	return SUCCESS;
}

// New data goes in the back of the deque
int sharedCache<CarData*>::feedCache()
{
	std::unique_lock<std::shared_mutex> ulock(smtx);

	CarData* received;
	int rc = feedQ->receive(&received);

	if (rc != SUCCESS) {
		ulock.unlock();
		return rc;
	}

	if (buffer.size() == maxSize) {
		delete buffer.front();
		buffer.pop_front();
	}

	buffer.push_back(received);

	ulock.unlock();
	return SUCCESS;
}

int sharedCache<CarData*>::updateCache()
{
	std::unique_lock<std::shared_mutex> ulock(smtx);
	ulock.lock();

	int rc = SUCCESS;

	int ind, findRC;
	CarData* tempData;

	while (updateQ->receiveQsize() > 0) {
		updateQ->receiveQfront(&tempData);

		findRC = findItem(tempData, &ind);
		if (findRC == SUCCESS) {
			buffer.at(ind) = tempData;
		}
		else {
			rc |= findRC;
		}
	}

	ulock.unlock();
	return rc;
}

int sharedCache<CarData*>::readCache(cacheIter* startIter, cacheIter* endIter){
	if ((startIter == nullptr) || (endIter == nullptr)) {
		return NULLPTRERR;
	}

	slock->lock();

	if (buffer.size() == 0) {
		slock->unlock();
		return EMPTYQUEUE;
	}

	*startIter = buffer.begin();
	*endIter = buffer.end();

	return SUCCESS;
}

int sharedCache<CarData*>::readCache(cacheIter* startIter, cacheIter* endIter, unsigned int length){
	if ((startIter == nullptr) || (endIter == nullptr)) {
		return NULLPTRERR;
	}

	if ((length > maxSize) || (length > buffer.size())) {
		return OUTOFRANGE;
	}

	slock->lock();

	if (buffer.size() == 0) {
		slock->unlock();
		return EMPTYQUEUE;
	}

	*startIter = buffer.begin();
	*endIter = buffer.begin() + length;

	return SUCCESS;
}

int sharedCache<CarData*>::releaseReadLock(){
	slock->unlock();
	return SUCCESS;
}

// TODO
bool sharedCache<CarData*>::newRawData()
{
	return true;
}

bool sharedCache<CarData*>::newAnalyzedData()
{
	return true;
}

template <>
int sharedCache<CarData*>::findItem(CarData* toFind, int* ind)
{
	if ((toFind == nullptr) || (ind == nullptr)) {
		return NULLPTRERR;
	}
	
	int left = 0;
	int right = buffer.size();
	int middle;

	unsigned long toFindTimeStamp, searchTimeStamp;
	toFind->get("TM", &toFindTimeStamp);

	while (left <= right) {
		middle = (left + right) / 2;

		buffer.at(middle)->get("TM", &searchTimeStamp);

		if (searchTimeStamp == toFindTimeStamp) {
			*ind = middle;
			return SUCCESS;
		}
		else if (searchTimeStamp > toFindTimeStamp) {
			right = middle - 1;
		}
		else {
			left = middle + 1;
		}
	}

	return NOTFOUND;
}
