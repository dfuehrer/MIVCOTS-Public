#include "sharedCache.h"

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
		return ERR_NULLPTR;
	}

	feedQ = _feedQ;
	updateQ = _updateQ;

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
	else if (buffer.size() == 0) {
		buffer.push_front(received);
		ulock.unlock();
		return SUCCESS;
	}

	/*
	long newTime, insertTime;
	std::deque<CarData*>::reverse_iterator insertIter = buffer.rbegin();

	received->get("TM", &newTime);
	(*insertIter)->get("TM", &insertTime);

	while (newTime < insertTime) {
		++insertIter;
		if (insertIter == buffer.rend()) {
			buffer.push_front(received);
			ulock.unlock();
			return SUCCESS;
		}
		(*insertIter)->get("TM", &insertTime);
	}
	
	buffer.insert(insertIter.base(), received);
	*/
	
	buffer.push_back(received);
	ulock.unlock();
	return SUCCESS;
}

int sharedCache<CarData*>::updateCache()
{
	std::unique_lock<std::shared_mutex> ulock(smtx);

	int rc = SUCCESS;

	int ind, findRC;
	CarData* tempData;

	while (updateQ->receiveQsize() > 0) {
		updateQ->receive(&tempData);

		findRC = findItem(tempData, &ind);
		if (findRC == SUCCESS) {
			delete buffer.at(ind);
			buffer.at(ind) = tempData;

		}
		else {
			rc |= findRC;
		}
	}

	ulock.unlock();
	return rc;
}

int sharedCache<CarData*>::acquireReadLock(std::shared_lock<std::shared_mutex> *toLock)
{
	*toLock = std::shared_lock<std::shared_mutex>(smtx);
	
	return SUCCESS;
}

int sharedCache<CarData*>::readCache(cacheIter* startIter, cacheIter* endIter){
	if ((startIter == nullptr) || (endIter == nullptr)) {
		return ERR_NULLPTR;
	}

	if (buffer.size() == 0) {
		
		return ERR_EMPTYQUEUE;
	}

	*startIter = buffer.begin();
	*endIter = buffer.end();

	return SUCCESS;
}

int sharedCache<CarData*>::readCache(cacheIter* startIter, cacheIter* endIter, unsigned int length){
	if ((startIter == nullptr) || (endIter == nullptr)) {
		return ERR_NULLPTR;
	}

	if ((length > maxSize) || (length > buffer.size())) {
		return ERR_OUTOFRANGE;
	}

	if (buffer.size() == 0) {
		
		return ERR_EMPTYQUEUE;
	}

	*startIter = buffer.begin();
	*endIter = buffer.begin() + length;

	return SUCCESS;
}

int sharedCache<CarData*>::releaseReadLock(std::shared_lock<std::shared_mutex>* toUnlock){
	toUnlock->unlock();
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
		return ERR_NULLPTR;
	}
	
	if (buffer.size() == 0) {
		return ERR_EMPTYCACHE;
	}
	
	int left = 0;
	int right = buffer.size() - 1;
	int middle;

	unsigned long toFindTimeStamp, searchTimeStamp;
	toFind->get(TIME_S, &toFindTimeStamp);

	while (left <= right) {
		middle = (left + right) / 2;

		buffer.at(middle)->get(TIME_S, &searchTimeStamp);

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

	return ERR_NOTFOUND;
}


int sharedCache<CarData*>::find(CarData*toFind, cacheIter* iter) {
	int index = 0;
	int returnCode = SUCCESS;
	returnCode = this->findItem(toFind, &index);
	*iter = this->buffer.begin() + index;
	return returnCode;
}


int sharedCache<CarData*>::find(CarData * toFind, int * ind)
{
	return this->findItem(toFind, ind);
	
}
