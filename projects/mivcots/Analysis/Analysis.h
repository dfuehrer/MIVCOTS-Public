#pragma once


#include <vector>
#include "InterThreadComm/endpoint.h"
#include "InterThreadComm/sharedCache.h"
#include "CarPool/CarPool.h"
#include "CarData/CarData.h"


class Analysis
{
public:
	Analysis();
	~Analysis();
	/* 
		Initialize Parent Analysis Object:
		Arguments:
			a pointer to the Box Cache
			a pointer to the Car Cache
			a pointer to the Update Queue, where we will be putting aggregated data
			the Car Pool, where we get cars from
		Returns:
			status code
	*/
	int init(sharedCache<CarData *> boxCache, sharedCache<CarData *> carCache, endpoint<CarData *> updateQueue, CarPool * carPool);
	


};

