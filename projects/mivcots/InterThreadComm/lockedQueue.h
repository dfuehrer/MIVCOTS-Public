#ifndef LOCKED_QUEUE_H
#define LOCKED_QUEUE_H

#include <mutex>
#include <queue>

#include "error_codes.h"

// Thread safe Queue class
// Defined in the header because templates are spooky

template <class T> class lockedQueue
{
public:
	lockedQueue() {}
	~lockedQueue() {}

	int push(T toPush)
	{
		std::lock_guard<std::mutex> lock(Qmutex);
		Q.push(toPush);

		return SUCCESS;
	}

	int pop(T* destination)
	{
		std::lock_guard<std::mutex> lock(Qmutex);
		if (Q.empty()) {
			return ERR_EMPTYQUEUE;
		}
		else if (destination == nullptr) {
			return ERR_NULLPTR;
		}
		else {
			*destination = Q.front();
			Q.pop();

			return SUCCESS;
		}
	}

	int front(T* destination)
	{
		std::lock_guard<std::mutex> lock(Qmutex);
		if (Q.empty()) {
			return ERR_EMPTYQUEUE;
		}
		else if (destination == nullptr) {
			return ERR_NULLPTR;
		}
		else {
			*destination = Q.front();
			return SUCCESS;
		}
	}

	int size()
	{
		std::lock_guard<std::mutex> lock(Qmutex);
		return Q.size();
	}

protected:
	std::queue<T> Q;
	std::mutex Qmutex;
};
#endif //LOCKED_QUEUE_H
