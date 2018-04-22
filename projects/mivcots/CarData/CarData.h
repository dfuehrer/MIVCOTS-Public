#pragma once

#include <map>
#include <string>
#include <vector>
#include <wx/wx.h>
#include "error_codes.h"

typedef struct dataValue_st {
	union data_u {
		long s;
		unsigned long u;
		double d;
	};

	data_u data;

} dataValue;

class CarData
{
public:
	CarData();
	~CarData();

	int addKey(std::string key);

	int set(std::string key, long newValue);
	int set(std::string key, unsigned long newValue);
	int set(std::string key, double newValue);

	int get(std::string key, long* dest);
	int get(std::string key, unsigned long* dest);
	int get(std::string key, double* dest);
	
	CarData & operator+=(CarData & rhs);

	void printCar();

protected:
	typedef std::map<std::string, dataValue> dataMap_t;
	dataMap_t dataMap;

	inline int find_key(std::string key, dataMap_t::iterator* iter);
	
};

// This feels wrong, but apparently it works...
struct carTimeStampCompareLess {
	bool operator()(CarData * lhs, CarData * rhs) {
		long lhsTimeStamp, rhsTimeStamp;
		lhs->get(std::string("TMS"), &lhsTimeStamp);
		rhs->get(std::string("TMS"), &rhsTimeStamp);
		return lhsTimeStamp < rhsTimeStamp;
	}
};

/*
#define KEYLENGTH 3

typedef struct dataValue_st {
	union data_u {
		long s;
		unsigned long u;
		double d;
	};
	
	data_u data;

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

	int addKey(char* key);
	int addKey(std::string key);
	int initialize(std::vector<char*>* keys, bool clearMap);

	int set(char* key, long newValue);
	int set(char* key, unsigned long newValue);
	int set(char* key, double newValue);

	int get(char* key, long* dest);
	int get(char* key, unsigned long* dest);
	int get(char* key, double* dest);	

	void printCar();

protected:
	typedef std::map<char*, dataValue, cStrCmp> dataMap_t;
	dataMap_t dataMap;

	inline int find_key(char* key, dataMap_t::iterator* iter);
};
*/
