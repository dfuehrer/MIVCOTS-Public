#include "InterThreadComm.h"

template <class T1, class T2> InterThreadComm<T1, T2>::InterThreadComm()
{
	forwardQ = new lockedQueue<T1>;
	backwardQ = new lockedQueue<T2>;
	endpoint1 = new endpoint<T1, T2>(forwardQ, backwardQ);
	endpoint2 = new endpoint<T2, T1>(backwardQ, forwardQ);
}

template <class T1, class T2> InterThreadComm<T1, T2>::~InterThreadComm()
{
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

template <class T1, class T2> endpoint<T1, T2>* InterThreadComm<T1, T2>::getEndpoint1()
{
	return endpoint1;
}

template <class T1, class T2> endpoint<T2, T1>* InterThreadComm<T1, T2>::getEndpoint2()
{
	return endpoint2;
}
