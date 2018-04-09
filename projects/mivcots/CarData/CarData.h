#pragma once

#include <map>
#include <vector>

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

protected:
	typedef std::map<char*, dataValue, cStrCmp> dataMap_t;
	dataMap_t dataMap;

	inline int find_key(char* key, dataMap_t::iterator* iter);
};

