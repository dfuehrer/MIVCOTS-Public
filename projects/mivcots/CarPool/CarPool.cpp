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

int CarPool::copyCar(CarData ** dest, CarData * toCopy)
{
	if ((dest == nullptr) || (toCopy == nullptr)) {
		return ERR_NULLPTR;
	}

	*dest = new CarData(toCopy);
	return SUCCESS;
}

int CarPool::releaseCar(CarData * toRelease)
{
	if (toRelease == nullptr) {
		return ERR_NULLPTR;
	}

	delete toRelease;
	return SUCCESS;
}
