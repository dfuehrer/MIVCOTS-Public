#pragma once

#include <shared_mutex>
#include <deque>

#include "endpoint.h"
#include "error_codes.h"

template <class T> class sharedCache
{
public:
	sharedCache();
	~sharedCache();

	int initialize(unsigned int _maxSize,
				   endpoint<T>* _feedQ,
				   endpoint<T>* _updateQ
				  );

	int feedCache();
	int updateCache();
	int readCache();
private:
	unsigned int maxSize;
	endpoint<T>* feedQ;
	endpoint<T>* updateQ;

	std::deque<T> buffer;
	std::shared_mutex smtx;
};
