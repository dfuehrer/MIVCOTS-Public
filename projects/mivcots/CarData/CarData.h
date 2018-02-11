#pragma once

#include <map>

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

struct cStrCmp {
	bool operator()(const char* lhs, const char* rhs) const{
		return strncmp(lhs, rhs, KEYLENGTH) < 0;
	}
};

class CarData
{
public:
	CarData();
	~CarData();

	

protected:
	std::map<char*, dataValue, cStrCmp> dataMap;
};

