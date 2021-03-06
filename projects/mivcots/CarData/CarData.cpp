#include "error_codes.h"
#include "CarData.h"

CarData::CarData()
{
}

CarData::CarData(CarData * toCopy)
{
	if (toCopy == nullptr) {
		wxLogDebug("Copying a nullptr cardata");
		return;
	}

	dataMap = dataMap_t(toCopy->dataMap.begin(), toCopy->dataMap.end());
}

CarData::~CarData()
{
}

int CarData::addKey(std::string key)
{
	if (dataMap.find(key) == dataMap.end()) {
		dataMap[key].data.u = 0;
		return SUCCESS;
	}
	else {
		return ERR_ELEMENTEXISTS;
	}
}

int CarData::set(std::string key, long newValue)
{
	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		iter->second.data.s = newValue;
	}

	return rc;
}

int CarData::set(std::string key, unsigned long newValue)
{
	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		iter->second.data.u = newValue;
	}

	return rc;
}

int CarData::set(std::string key, double newValue)
{
	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		iter->second.data.d = newValue;
	}

	return rc;
}



int CarData::get(std::string key, long * dest)
{
	if (dest == nullptr) {
		return ERR_NULLPTR;
	}

	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		*dest = iter->second.data.s;
	}

	return rc;
}

int CarData::get(std::string key, unsigned long * dest)
{
	if (dest == nullptr) {
		return ERR_NULLPTR;
	}

	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		*dest = iter->second.data.u;
	}

	return rc;
}

int CarData::get(std::string key, double * dest)
{
	if (dest == nullptr) {
		return ERR_NULLPTR;
	}

	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		*dest = iter->second.data.d;
	}

	return rc;
}

// Adds all elements of rhs to lhs
// rhs wins all conflicts
// if rhs and lhs have same members, functions as = operator
CarData & CarData::operator+=(CarData & rhs)
{
	for (dataMap_t::iterator iter = rhs.dataMap.begin(); iter != rhs.dataMap.end(); ++iter) {
		this->dataMap[iter->first] = iter->second;
	}
	return *this;
}

void CarData::printCar()
{
	for (dataMap_t::iterator iter = dataMap.begin(); iter != dataMap.end(); ++iter) {
		wxLogDebug("Key %s, Value %d", iter->first, iter->second.data.s);
	}
}

inline int CarData::find_key(std::string key, dataMap_t::iterator* iter)
{
	if (iter == nullptr) {
		return ERR_NULLPTR;
	}

	*iter = dataMap.find(key);

	if (*iter == dataMap.end()) {
		return ERR_OUTOFRANGE;
	}
	else {
		return SUCCESS;
	}
}



/*
int CarData::addKey(char* key) 
{
	if (key == nullptr) {
		return ERR_NULLPTR;
	}

	if (dataMap.find(key) == dataMap.end()) {
		dataMap[key].data.u = 0;
		return SUCCESS;
	}
	else {
		return ERR_ELEMENTEXISTS;
	}
}

int CarData::addKey(std::string key)
{
	return addKey((char *)key.c_str());
}

int CarData::initialize(std::vector<char*>* keys, bool clearMap)
{
	if (clearMap) {
		dataMap.clear();
	}
	
	if (keys == nullptr) {
		return ERR_NULLPTR;
	}

	int rc = SUCCESS;

	for (char* key : *keys) {
		rc |= addKey(key);
	}

	return rc;
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
		return ERR_NULLPTR;
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
		return ERR_NULLPTR;
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
		return ERR_NULLPTR;
	}

	dataMap_t::iterator iter;
	int rc = find_key(key, &iter);

	if (rc == SUCCESS) {
		*dest = iter->second.data.d;
	}

	return rc;
}

void CarData::printCar()
{
	for (dataMap_t::iterator iter = dataMap.begin(); iter != dataMap.end(); ++iter) {
		wxLogDebug("Key %s, Value %d", iter->first, iter->second.data.s);
	}
}

inline int CarData::find_key(char* key, dataMap_t::iterator* iter) 
{
	if ((iter == nullptr) || (key == nullptr)) {
		return ERR_NULLPTR;
	}

	*iter = dataMap.find(key);

	if (*iter == dataMap.end()) {
		return ERR_OUTOFRANGE;
	}
	else {
		return SUCCESS;
	}
}
*/