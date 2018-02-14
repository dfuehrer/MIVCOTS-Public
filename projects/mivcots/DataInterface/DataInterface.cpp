#include "DataInterface.h"



DataInterface::DataInterface()
{

}

DataInterface::~DataInterface()
{
	stop();

	if (dataSource != nullptr){
		dataSource->flush();
		dataSource->close();
		delete dataSource;
	}
	
}

void DataInterface::initialize(std::string portName, long int baud){
	dataSource = new serial::Serial(portName, baud);
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
	while(isRunning){
		
	}
}

CarData DataInterface::parseString(std::string toParse)
{
	return CarData();
}

