#pragma once

#include "CarData.h"
#include "error_codes.h"
class CarPool {
public:
	CarPool();
	~CarPool();

	int getCar(CarData** dest);
private:

};