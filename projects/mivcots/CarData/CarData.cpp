#include "error_codes.h"
#include "CarData.h"

CarData::CarData()
{
}

CarData::~CarData()
{
}

int CarData::set(char* key, long newValue)
{
	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		iter->second.data.s = newValue;
	}

	return rc;
}

int CarData::set(char* key, unsigned long newValue)
{
	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		iter->second.data.u = newValue;
	}

	return rc;
}

int CarData::set(char* key, double newValue)
{
	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		iter->second.data.d = newValue;
	}

	return rc;
}

int CarData::get(char* key, long* dest)
{
	if (dest == nullptr) {
		return NULLPTRERR;
	}

	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		*dest = iter->second.data.s;
	}

	return rc;
}

int CarData::get(char* key, unsigned long* dest)
{
	if (dest == nullptr) {
		return NULLPTRERR;
	}

	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		*dest = iter->second.data.u;
	}

	return rc;
}

int CarData::get(char* key, double* dest)
{
	if (dest == nullptr) {
		return NULLPTRERR;
	}

	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		*dest = iter->second.data.d;
	}

	return rc;
}

inline int CarData::find_key(char* key, dataMap_t::iterator* iter) {

	if ((iter == nullptr) || (key == nullptr)) {
		return NULLPTRERR;
	}

	*iter = dataMap.find(key);

	if (*iter == dataMap.end()) {
		return OUTOFRANGE;
	}
	else {
		return SUCCESS;
	}
}