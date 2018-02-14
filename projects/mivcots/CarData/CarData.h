#pragma once

#include <map>

#define KEYLENGTH 3

typedef struct dataValue_st {
	union unprocessed {
		long s;
		unsigned long u;
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

	int set(char* key, long newValue);
	int set(char* key, unsigned long newValue);
	int set(char* key, double newValue);

	int get(char* key, long* dest);
	int get(char* key, unsigned long* dest);
	int get(char* key, double* dest);	

protected:
	std::map<char*, dataValue, cStrCmp> dataMap;
};

