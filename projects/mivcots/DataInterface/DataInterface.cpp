#include "DataInterface.h"
#include "../DataArrays/DataArrays.h"

#include <cstring>
#include <vector>
#include <string>

#define HEADER_DELIM ','
#define HEADER_END ':'

// TODO i have to make json files to describe my data structs and they contain the info on what the number was scaled by (theoretically) in the arduino so those jsons should be read here to know how to rescale the numbers into doubles
DataInterface::DataInterface()
{
	serialPort = nullptr;
}

DataInterface::~DataInterface()
{
	stop();
}

int DataInterface::initialize(std::string portName, long int baud, endpoint<CarData*>* _outputQ, CarPool* _CarSource)
{
	outputQ = _outputQ;
	CarSource = _CarSource;
	
	if (serialPort == nullptr) {
		try {
			serialPort = new serial::Serial(portName, baud);
		}
		catch (...) {
			wxLogWarning("Failed to open serial port %s", portName);
			return ERR_INITERR;
		}
	}
	else {
		if (serialPort->isOpen()) {
			serialPort->flush();
			serialPort->close();
		}
		delete serialPort;
		serialPort = new serial::Serial(portName, baud);
	}
	
	if (serialPort == nullptr) {
		wxLogWarning("Failed to open serial port %s", portName);
		return ERR_INITERR;
	}

	serialPort->setTimeout(500, 0, 500, 0, 0);
	serialPort->setParity(serial::parity_even);
	wxLogMessage("Opened serial port %s", portName);
	return SUCCESS;
}

int DataInterface::start()
{
	isRunning.store(true, std::memory_order_relaxed);
	serialThread = std::thread(&DataInterface::runSerialThread, this);

	if (serialThread.joinable()) {
		return SUCCESS;
	}
	else {
		return ERR_INITERR;
	}
}

int DataInterface::stop()
{
	isRunning.store(false, std::memory_order_relaxed);
	if (serialThread.joinable()) {
		serialThread.join();
	}

	if (serialPort != nullptr) {
		if (serialPort->isOpen()) {
			serialPort->flush();
			serialPort->close();
		}
		delete serialPort;
		serialPort = nullptr;
	}

	return SUCCESS;
}

bool DataInterface::isSerialRunning()
{
	return isRunning.load(std::memory_order_relaxed);
}

bool DataInterface::portOpen()
{
	if (serialPort == nullptr) {
		return false;
	}
	else {
		return serialPort->isOpen();
	}
}

void DataInterface::runSerialThread()
{
	std::string readStr;
	CarData* tmpCarData;
	int rc;

	while (isRunning) {
		rc = SUCCESS;

        // TODO change this to something that actually works
        // new idea:
        //  check that theres enough bytes to see the length of the buffer
        //  peek at the length to know how much we need to go
        //  check if theres enough in the buffer to grab based on the length
        //  if there is then read that length of buffer
        //  if not just continue till there is
		readStr = serialPort->readline();
		if (readStr.empty()) {
			continue; // Read timed out - probably no data
		}
		
		rc |= CarSource->getCar(&tmpCarData);
		rc |= parseString(readStr, &tmpCarData);

		
		//tmpCarData->printCar();
		if (rc != SUCCESS) {
			std::string errormsg = "Serial Thread Error with code: " + std::to_string(rc);
			CarSource->releaseCar(tmpCarData);
			wxLogDebug(_(errormsg));
		}
		else {
			outputQ->send(tmpCarData);
			//wxLogDebug("Data Interface sent a cardata");
		}
	}
}

int DataInterface::parseString(std::string toParse, CarData** parsed)
{
	// Parsing stuff
	size_t delimPosRight = toParse.find(DELIMITER); // Find the first comma
	size_t delimPosLeft = 0;
	std::string tmpKey, tmpValue;
	long tmpLong, numPairs;
	int rc, numFields;

	// First count the number of fields in the message
	countFields(toParse, numFields);

	// Number of keys + number of values + the end of msg symbol 
	// Should be odd
	if ((numFields % 2) == 0) {
		return ERR_INVALIDMSGFORMAT;
	}

	// Calculate the number of pairs
	numPairs = (numFields - 1) / 2;

	// Check the beginning of message symbol
	if (toParse.substr(delimPosLeft, delimPosRight) != "#") {
		return ERR_INVALIDMSGFORMAT;
	}

	// Do the first four fields manually
	// Replace # with message number key
	tmpKey = MESSAGE_NUMBER_S;
	tmpKey.pop_back();			// Remove the S so we can add it again later
	// Assigns the message number field
	rc = parseLoop(toParse, delimPosLeft, delimPosRight, tmpKey, tmpValue, tmpLong, parsed);
	if (rc != SUCCESS){
		return rc;
	}

	// Assigns the Number of fields
	rc = parseLoop(toParse, delimPosLeft, delimPosRight, tmpKey, tmpValue, tmpLong, parsed);
	if (rc != SUCCESS) {
		return rc;
	}

	if (tmpLong != numPairs) {
		return ERR_INVALIDMSGFORMAT;
	}

	// loop through the remainder
	while (tmpKey != ENDOFMSG) {
		rc = parseLoop(toParse, delimPosLeft, delimPosRight, tmpKey, tmpValue, tmpLong, parsed);
		if (rc != SUCCESS) {
			return rc;
		}
	}

	long parsedTime;
	(*parsed)->get(TIME_S, &parsedTime);

	if (parsedTime < 0) {
		return ERR_TIME;
	}

	(*parsed)->addKey(ANALYSIS_COUNT_U);
	(*parsed)->set(ANALYSIS_COUNT_U,(unsigned long)0);

	//long numFields

	//(*parsed)->printCar();
	return SUCCESS;
}

int DataInterface::countFields(std::string & toParse, int & numFields)
{
	size_t delimPosRight = toParse.find(DELIMITER); // Find the first comma
	size_t delimPosLeft = 0;
	
	if (delimPosRight == std::string::npos) {
		numFields = 0;
		return ERR_INVALIDMSGFORMAT;
	}
	else {
		numFields = 1; // We found the first field. Now start counting
	}

	while (delimPosRight != std::string::npos) {
		advanceDelimiters(delimPosLeft, delimPosRight, toParse);
		++numFields;
	}
	
	return SUCCESS;
}

int DataInterface::advanceDelimiters(size_t & left, size_t & right, std::string & toParse)
{
	left = right + 1;
	right = toParse.find(DELIMITER, left);
	
	return SUCCESS;
}

int DataInterface::convertLong(std::string &toConvert, long &output)
{
	try {
		output = std::stol(toConvert);
	}
	catch (...) {
		return ERR_VALUEERR;
	}

	return SUCCESS;
}

int DataInterface::parseLoop(std::string & toParse, size_t & left, size_t & right, std::string & tmpKey, std::string & tmpValue, long & tmpLong, CarData ** parsed)
{
	// First add the key to the CarData
	(*parsed)->addKey(tmpKey + "S");

	// Advance to the next field, which should be a value
	advanceDelimiters(left, right, toParse);

	// Extract value to a substring
	tmpValue = toParse.substr(left, right - left);

	//wxLogDebug("Reading value: %s", tmpValue);

	// Convert to long
	int convRC = convertLong(tmpValue, tmpLong);

	if (convRC != SUCCESS) {
		return convRC;
	}

	// Set the value in the carData
	(*parsed)->set(tmpKey + "S", tmpLong);

	// In case the message doesn't have an end of msg symbol
	if (right == std::string::npos) {
		wxLogMessage("Missing End-of-Message symbol");
		return ERR_INVALIDMSGFORMAT;
	}

	// Advance to the next field, which should be a key
	advanceDelimiters(left, right, toParse);

	// Extract key to a substring
	tmpKey = toParse.substr(left, right - left);
	trim(tmpKey);

	return SUCCESS;

	//wxLogDebug("Reading Key: %s", tmpKey);
}









int DataInterface::parseData(std::string &toParse, size_t & left, CarData ** parsed){
    uint8_t * datas = (uint8_t *)(toParse.c_str() + left);
    uint8_t * d = datas;
    // this line really lets you know how this function will be written
    // i could write it to be readable and maintainable and debuggable but instead we have *d++ in an if statement condition
    if(*d++ != '{'){
        return ERR_INVALIDMSGFORMAT;
    }
    char key[5] = "";
    std::memcpy(key, (char *)datas, 4);
    std::string ID(key);
    key[4] = '\0';
    d += 4;
    if(*d++ != HEADER_DELIM){
        return ERR_INVALIDMSGFORMAT;
    }
    uint16_t size = *(uint16_t *)d;
    d += 2;
    if(*d++ != HEADER_END){
        return ERR_INVALIDMSGFORMAT;
    }
    if(d[size] != ';'){
        return ERR_INVALIDMSGFORMAT;
    }
    if(d[size + 10] != '}'){
        return ERR_INVALIDMSGFORMAT;
    }
    // get the right array to know how to unload the data
    // we get the right array by the first 3 characters in the id (the key)
    const std::vector<std::pair<std::string, int>> dataArray = dataArrayMap.at(key);
    for(const auto & varEl = dataArray.cbegin(); varEl != dataArray.cend(); varEl++){
        std::string varName = varEl->first;
        int length = varEl->second;
        (*parsed)->addKey(varName + ID + "S");
        uint32_t value = -1;
        switch(length){
            case 4:
                value = *(uint32_t *)d; break;
            case 2:
                value = *(uint16_t *)d; break;
            case 1:
                value = *(uint8_t  *)d; break;
            default:
                return ERR_INVALIDMSGFORMAT;
        }
        d += length;
        (*parsed)->set(varName + ID + "S", value);
    }
    d++;
    uint32_t millisPassed = *(uint32_t *)d;
    d += 4;
    if(*d++ != '.'){
        return ERR_INVALIDMSGFORMAT;
    }
    uint32_t sum = 0;
    const uint32_t * tmpval;
    for(tmpval = (uint32_t *)(datas + 1); (uint8_t *)(tmpval + 1) < d; tmpval++){
        sum += *tmpval;
    }
    if((d - datas - 1) % 4){
        for(uint8_t * tmp8 = (uint8_t *)tmpval, shiftBy = 24; *tmp8 != '}'; tmp8++, shiftBy -= 8){
            sum += (uint32_t)*tmp8 << (shiftBy);
            if(*tmp8 == '.'){
                break;
            }
        }
    }
    uint32_t checksum = *(uint32_t *)d;
    if(checksum != sum){
        return ERR_VALUEERR;
    }
    d += 4;

    left += (++d - datas);
    return SUCCESS;
}


int DataInterface::parseHeader(std::string &toParse, CarData ** parsed){
    uint8_t * datas = (uint8_t *)(toParse.c_str());
    uint8_t * d = datas + 1;
    // TODO acutall do stuff with these 32bit ints im making
    uint32_t messageNumber = *(uint32_t *)d;
    d += 4;
    if(*d++ != HEADER_DELIM){
        return ERR_INVALIDMSGFORMAT;
    }
    uint32_t GPSDate = *(uint32_t *)d;
    d += 4;
    if(*d++ != '-'){
        return ERR_INVALIDMSGFORMAT;
    }
    uint32_t GPSTime = *(uint32_t *)d;
    d += 4;
    if(*d++ != HEADER_DELIM){
        return ERR_INVALIDMSGFORMAT;
    }
    uint32_t totalsize = *(uint32_t *)d;
    d += 4;
    if(*d++ != HEADER_END){
        return ERR_INVALIDMSGFORMAT;
    }
    // TODO add putting the data into the parsed CarData object with the set and stuff
	//(*parsed)->addKey(tmpKey + "S");
    return SUCCESS;
}

int DataInterface::parseFooter(std::string &toParse, size_t & left, CarData ** parsed){
    uint8_t * d = (uint8_t *)(toParse.c_str() + left);
    if(*d++ != '|'){
        return ERR_INVALIDMSGFORMAT;
    }
    uint32_t checksum = *(uint32_t *)d;
    d += 4;
    if(*d++ != ENDOFMSG){
        return ERR_INVALIDMSGFORMAT;
    }
    // TODO figure out doing the checksum checking
}


int DataInterface::parseString(std::string toParse, CarData** parsed){
    uint8_t * d = (uint8_t *)(toParse.c_str());
    if(*d++ != '$'){
        return ERR_INVALIDMSGFORMAT;
    }
    parseHeader(toParse, parsed);
    size_t left = 16;
    d += left;
    for(; *d == '{'; d += left){
        parseData(toParse, left, parsed);
    }
    parseFooter(toParse, left, parsed);
}




