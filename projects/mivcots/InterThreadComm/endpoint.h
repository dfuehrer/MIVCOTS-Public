#ifndef ENDPOINT_H
#define ENDPOINT_H

#include "lockedQueue.h"

// Interface for two directional inter-thread communications
// Defined in the header because templates are spooky

template <class T1, class T2 = T1> class endpoint
{
public:
	endpoint(lockedQueue<T1>* _forwardQ, lockedQueue<T2>* _backwardQ) : 
		forwardQ(_forwardQ), backwardQ(_backwardQ) {}
	~endpoint() {}

	int send(T1 toSend) { return forwardQ->push(toSend); }
	int receive(T2* destination) { return backwardQ->pop(destination); }

	int sendQsize() { return forwardQ->size(); }
	int receiveQsize() { return backwardQ->size(); }
	
	int receiveQfront(T2* destination) { return backwardQ->front(destination); }

protected:
	lockedQueue<T1>* forwardQ;
	lockedQueue<T2>* backwardQ;
};
#endif //ENDPOINT_H
