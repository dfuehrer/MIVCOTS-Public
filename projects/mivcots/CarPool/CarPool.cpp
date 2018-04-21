#include "CarPool.h"

CarPool::CarPool()
{
}

CarPool::~CarPool()
{
}

int CarPool::getCar(CarData** dest)
{
	if (dest == nullptr) {
		return ERR_NULLPTR;
	}

	*dest = new CarData();

	return SUCCESS;
}
