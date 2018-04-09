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

int DataInterface::initialize(std::string portName, long int baud, endpoint<CarData*>* _outputQ)
{
	outputQ = _outputQ;
	
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
	while(isRunning){
		readStr = serialPort->readline();
		outputQ->send(parseString(readStr));
	}
}

int DataInterface::parseString(std::string toParse, CarData** parsed)
{
	CarData* newCar = new CarData();

	size_t delimPos = toParse.find(DELIMITER);

	while (delimPos != std::string::npos) {

	}


	return newCar;
}

