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
		//readStr = "#,5,ID,14,ST,3,LT,12345,LG,54321";
		rc |= CarSource->getCar(&tmpCarData);
		rc |= parseString(readStr, &tmpCarData);

		
		//tmpCarData->printCar();
		if (rc != SUCCESS) {
			std::string errormsg = "Serial Thread Error with code: " + std::to_string(rc);
			wxLogWarning(_(errormsg));
		}
		else {
			outputQ->send(tmpCarData);
			//wxLogDebug("Data Interface sent a cardata");
		}
	}
}

// Test string: "#,5,ID,14,ST,3,LT,12345,LG,54321,!"
int DataInterface::parseString(std::string toParse, CarData** parsed)
{
	//wxLogDebug("Received: %s", toParse);

	// Parsing stuff
	size_t delimPosRight = toParse.find(DELIMITER); // Find the first comma
	size_t delimPosLeft = 0;
	std::string tmpKey, tmpValue;
	long tmpLong;

	if (toParse.substr(delimPosLeft, delimPosRight) != "#") {
		return ERR_INVALIDMSGFORMAT;
	}

	// Do the first field manually
	// Replace # with message number key
	tmpKey = MESSAGE_NUMBER_S;
	tmpKey.pop_back();			// Remove the S so we can add it again later

	// loop through the remainder
	while (tmpKey != ENDOFMSG) {
		// First add the key to the CarData
		(*parsed)->addKey(tmpKey + "S");

		// Advance to the next field, which should be a value
		delimPosLeft = delimPosRight + 1;
		delimPosRight = toParse.find(DELIMITER, delimPosLeft);

		// Extract value to a substring
		tmpValue = toParse.substr(delimPosLeft, delimPosRight - delimPosLeft);

		//wxLogDebug("Reading value: %s", tmpValue);

		// Convert to long
		try {
			tmpLong = std::stol(tmpValue);
		}
		catch (...) {
			return ERR_VALUEERR;
		}

		// Set the value in the carData
		(*parsed)->set(tmpKey + "S", tmpLong);

		// In case the message doesn't have an end of msg symbol
		if (delimPosRight == std::string::npos) {
			wxLogMessage("Missing End-of-Message symbol");
			break;
		}

		// Advance to the next field, which should be a key
		delimPosLeft = delimPosRight + 1;
		delimPosRight = toParse.find(DELIMITER, delimPosLeft);

		// Extract key to a substring
		tmpKey = toParse.substr(delimPosLeft, delimPosRight - delimPosLeft);
		trim(tmpKey);

		//wxLogDebug("Reading Key: %s", tmpKey);
	}

	(*parsed)->addKey(ANALYSIS_COUNT_U);
	(*parsed)->set(ANALYSIS_COUNT_U,(unsigned long)0);
	//(*parsed)->printCar();
	return SUCCESS;
}
