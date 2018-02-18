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
	InterThreadComm();
	~InterThreadComm();

	endpoint<T1, T2>* getEndpoint1();
	endpoint<T2, T1>* getEndpoint2();

private:
	endpoint<T1, T2>* endpoint1 = nullptr;
	endpoint<T2, T1>* endpoint2 = nullptr;
	lockedQueue<T1>* forwardQ = nullptr;
	lockedQueue<T2>* backwardQ = nullptr;
};
