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
		//trackUpdates(received);
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
	//trackUpdates(received);
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
			trackUpdates(tempData);
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

int sharedCache<CarData*>::readCacheUpdates(cacheIter* startIter, cacheIter* endIter, unsigned int updateCount)
{
	if ((startIter == nullptr) || (endIter == nullptr)) {
		return ERR_NULLPTR;
	}

	if ((int)updateCount > (int)latestUpdated.size() - 1) {
		return ERR_INVALID_UPDATECOUNT;
	}
	
	if (latestUpdated.at(updateCount) == nullptr) {
		return ERR_NOTFOUND;
	}

	// End Iter is one past the newest item with the right update count
	int endRC = find(latestUpdated.at(updateCount), endIter);
	++(*endIter);

	// Start Iter is either the beginning of the cache or the first item with the right update count
	unsigned long searchUpdateCount = updateCount + 1;
	// User asked for the highest possible update count, which start at the beginning of the cache
	// If the update hasn't happened at all yet, the function will return above
	if (searchUpdateCount == latestUpdated.size()) {
		*startIter = buffer.begin();
	}
	else {
		find(latestUpdated.at(searchUpdateCount), startIter);
		++(*startIter);
	}

	return SUCCESS;
}

template<>
int sharedCache<CarData*>::readLatestUpdate(cacheIter * iter, unsigned int updateCount)
{
	if (iter == nullptr) {
		return ERR_NULLPTR;
	}

	if ((int)updateCount > (int)latestUpdated.size() - 1) {
		return ERR_INVALID_UPDATECOUNT;
	}

	return find(latestUpdated.at(updateCount) , iter);
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

template<>
int sharedCache<CarData*>::findItem(CarData* toFind, int* ind)
{
	if ((toFind == nullptr) || (ind == nullptr)) {
		return ERR_NULLPTR;
	}
	
	if (buffer.size() == 0) {
		return ERR_EMPTYCACHE;
	}
	
	// Binary search
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

template<>
int sharedCache<CarData*>::find(CarData*toFind, cacheIter* iter) {
	int index = 0;
	int returnCode = SUCCESS;
	returnCode = this->findItem(toFind, &index);
	*iter = this->buffer.begin() + index;
	return returnCode;
}

template<>
int sharedCache<CarData*>::find(CarData * toFind, int * ind)
{
	return this->findItem(toFind, ind);
}

template<>
int sharedCache<CarData*>::trackUpdates(CarData* update)
{
	unsigned long analysisCount;
	long newUpdateTime, oldUpdateTime;
	update->get(ANALYSIS_COUNT_U, &analysisCount);
	update->get(TIME_S, &newUpdateTime);

	// We encountered an analysis count that we haven't seen before
	// Match the size of latestUpdated to the new analysis count
	while (latestUpdated.size() < analysisCount + 1) {
		latestUpdated.push_back(nullptr);
	}
	 
	// Haven't seen this analysis Count before
	if (latestUpdated.at(analysisCount) == nullptr) {
		latestUpdated.at(analysisCount) = update;
		return SUCCESS;
	}

	latestUpdated.at(analysisCount)->get(TIME_S, &oldUpdateTime);
	
	// Update has a newer timestamp
	if (oldUpdateTime < newUpdateTime) {
		latestUpdated.at(analysisCount) = update;
	}

	return SUCCESS;

}
