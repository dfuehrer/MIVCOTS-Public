#include "DataInterface.h"

DataInterface::DataInterface()
{

}

DataInterface::~DataInterface()
{
	stop();

	if (serialPort != nullptr){
		serialPort->flush();
		serialPort->close();
		delete serialPort;
	}
	
}

int DataInterface::initialize(std::string portName, long int baud, endpoint<CarData*>* _outputQ, CarPool* _CarSource)
{
	outputQ = _outputQ;
	CarSource = _CarSource;
	serialPort = new serial::Serial(portName, baud);
	
	if (serialPort == nullptr) {
		return INITERR;
	}

	return SUCCESS;
}

void DataInterface::start()
{
	isRunning.store(true, std::memory_order_relaxed);
	serialThread = std::thread(&DataInterface::runSerialThread, this);
}

void DataInterface::stop()
{
	isRunning.store(false, std::memory_order_relaxed);
	if (serialThread.joinable()){
		serialThread.join();
	}	
}

void DataInterface::runSerialThread()
{
	std::string readStr;
	CarData* tmpCarData;
	int rc;

	while(isRunning){
		rc = SUCCESS;

		readStr = serialPort->readline();
		
		tmpCarData = CarSource->getCar(&tmpCarData);
		rc |= parseString(readstr, &tempCarData);

		rc |= outputQ->send(tmpCarData);

		if (rc != SUCCESS) {
			std::string errormsg = "Serial Thread Error with code: " + to_string(rc);
			wxLogWarning(_(errormsg));
		}
	}
}

int DataInterface::parseString(std::string toParse, CarData** parsed)
{
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
	// Test string: "#,5,ID,14,ST,3,LT,12345,LG,54321,!"
	while ((tmpKey != ENDOFMSG) && (delimPosRight != std::string::npos) {
		// First add the key to the CarData
		newCar->addKey(tmpKey);

		// Advance to the next field, which should be a value
		delimPosLeft = delimPosRight + 1;
		delimPosRight = toParse.find(DELIMITER, delimPosLeft);

		// Extract value to a substring
		tmpValue = toParse.substr(delimPosLeft, delimPosRight - delimPosLeft);
		
		// Convert to long
		try{
			tmpLong = std::stol(tmpValue);
		}
		catch(...) {
			return VALUEERR;
		}

		// Set the value in the carData
		newCar->set(tmpKey.c_str(), tmpLong);

		// Advance to the next field, which should be a key
		delimPosLeft = delimPosRight
		delimPosRight = toParse.find(DELIMITER, delimPosLeft);

		// Extract key to a substring
		tmpKey = toParse.substr(delimPosLeft, delimPosRight - delimPosLeft);
	}

	return SUCCESS;
}
