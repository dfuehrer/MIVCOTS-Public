#include "DataInterface.h"

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
