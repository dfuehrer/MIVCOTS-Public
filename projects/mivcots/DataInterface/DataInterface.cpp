#include "DataInterface.h"

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

DataInterface::DataInterface()
{
	serialPort = nullptr;
}

DataInterface::~DataInterface()
{
	stop();

	if (serialPort != nullptr) {
		if (serialPort->isOpen()) {
			serialPort->flush();
			serialPort->close();
		}
		delete serialPort;
	}

}

int DataInterface::initialize(std::string portName, long int baud, endpoint<CarData*>* _outputQ, CarPool* _CarSource)
{
	outputQ = _outputQ;
	CarSource = _CarSource;
	
	if (serialPort == nullptr) {
		serialPort = new serial::Serial(portName, baud);
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
		return INITERR;
	}

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
		return INITERR;
	}
}

void DataInterface::stop()
{
	isRunning.store(false, std::memory_order_relaxed);
	if (serialThread.joinable()) {
		serialThread.join();
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
		//readStr = "#,5,ID,14,ST,3,LT,12345,LG,54321";
		rc |= CarSource->getCar(&tmpCarData);
		rc |= parseString(readStr, &tmpCarData);

		rc |= outputQ->send(tmpCarData);

		if (rc != SUCCESS) {
			std::string errormsg = "Serial Thread Error with code: " + std::to_string(rc);
			wxLogWarning(_(errormsg));
		}
	}
}

// Test string: "#,5,ID,14,ST,3,LT,12345,LG,54321,!"
int DataInterface::parseString(std::string toParse, CarData** parsed)
{
	wxLogDebug("Received: %s", toParse);
	CarData* newCar = new CarData();

	// Parsing stuff
	size_t delimPosRight = toParse.find(DELIMITER); // Find the first comma
	size_t delimPosLeft = 0;
	std::string tmpKey, tmpValue;
	long tmpLong;

	if (toParse.substr(delimPosLeft, delimPosRight) != "#") {
		return INVALIDMSGFORMAT;
	}

	// Do the first field manually
	// Replace # with MN for message number
	tmpKey = "MN";

	// loop through the remainder
	while (tmpKey != ENDOFMSG) {
		// First add the key to the CarData
		newCar->addKey((char *)(tmpKey.c_str()));

		// Advance to the next field, which should be a value
		delimPosLeft = delimPosRight + 1;
		delimPosRight = toParse.find(DELIMITER, delimPosLeft);

		// Extract value to a substring
		tmpValue = toParse.substr(delimPosLeft, delimPosRight - delimPosLeft);

		wxLogDebug("Reading value: %s", tmpValue);

		// Convert to long
		try {
			tmpLong = std::stol(tmpValue);
		}
		catch (...) {
			return VALUEERR;
		}

		// Set the value in the carData
		newCar->set((char *)(tmpKey.c_str()), tmpLong);

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

		wxLogDebug("Reading Key: %s", tmpKey);
	}

	return SUCCESS;
}
