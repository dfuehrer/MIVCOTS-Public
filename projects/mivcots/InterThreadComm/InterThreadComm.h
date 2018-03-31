#pragma once

#include "endpoint.h"

/*
This is a thread safe communication interface. Threads should
be assigned an endpoint by the MIVCOTS class, which they use 
to communicate.

T1 is the type being send by endpoint 1 and T2 is the type being
sent by endpoint 2.

		   -> forward Queue<T1>  ->
endpoint1						     endpoint2
		   <- backward Queue<T2> <-

Threads should NOT delete endpoints
*/

template <class T1, class T2 = T1> class InterThreadComm
{
public:
	InterThreadComm() {
		forwardQ = new lockedQueue<T1>;
		backwardQ = new lockedQueue<T2>;
		endpoint1 = new endpoint<T1, T2>(forwardQ, backwardQ);
		endpoint2 = new endpoint<T2, T1>(backwardQ, forwardQ);
	}
	~InterThreadComm() {
		if (forwardQ != nullptr) {
			delete forwardQ;
		}
		if (backwardQ != nullptr) {
			delete backwardQ;
		}
		if (endpoint1 != nullptr) {
			delete endpoint1;
		}
		if (endpoint2 != nullptr) {
			delete endpoint2;
		}
	}

endpoint<T1, T2>* getEndpoint1() { return endpoint1; }
endpoint<T2, T1>* getEndpoint2() { return endpoint2; }

private:
	endpoint<T1, T2>* endpoint1 = nullptr;
	endpoint<T2, T1>* endpoint2 = nullptr;
	lockedQueue<T1>* forwardQ = nullptr;
	lockedQueue<T2>* backwardQ = nullptr;
};
