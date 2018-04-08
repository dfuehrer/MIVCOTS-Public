#include "DataInterface.h"

DataInterface::DataInterface(std::string _tempString)
{
	tempString = _tempString;
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
	wxLogMessage(_(tempString));
	while(isRunning){
		
		//std::cout << tempString << std::endl;
	}
}

CarData* DataInterface::parseString(std::string toParse)
{
	CarData* newCar = new CarData();
	return newCar;
}

