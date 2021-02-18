#ifndef CAR_POOL_H
#define CAR_POOL_H

#include "CarData.h"
#include "error_codes.h"
class CarPool {
public:
	CarPool();
	~CarPool();

	int getCar(CarData** dest);
	int copyCar(CarData** dest, CarData* toCopy);

	int releaseCar(CarData* toRelease);
private:

};
#endif //CAR_POOL_H
