//#pragma once

#define KEYLENGTH 2

typedef struct dataValue_st {
	union unprocessed {
		long s;
		unsigned long u;
	};

	union processed {
		long i;
		double d;
	};
	
} dataValue;

class CarData
{
public:
	CarData();
	~CarData();
};

