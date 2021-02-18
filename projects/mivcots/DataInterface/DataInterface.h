#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H


#include <atomic>
#include <iostream>
#include <thread>
#include <wx/wx.h>
#include "CarData.h"
#include "CarPool.h"
#include "endpoint.h"
#include "key_defines.h"
#include "serial/serial.h"
#include "Utilities.h"

#define DELIMITER ","
#define ENDOFMSG "!"

class DataInterface
{
public:
	DataInterface();
	~DataInterface();

	int initialize(std::string portName, long int baud, endpoint<CarData*>* _outputQ, CarPool* _CarSource);
	int start();
	int stop();
	bool isSerialRunning();
	bool portOpen();

protected:
	std::atomic<bool> isRunning;
	std::thread serialThread;

	endpoint<CarData*>* outputQ;

	void runSerialThread();

	int parseString(std::string toParse, CarData** parsed);
	int countFields(std::string &toParse, int &numFields);
	int advanceDelimiters(size_t &left, size_t &right, std::string &toParse);
	int convertLong(std::string &toConvert, long &output);
	int parseLoop(std::string &toParse, size_t &left, size_t &right,
		std::string &tmpKey, std::string &tmpValue, long &tmpLong, CarData** parsed);

	serial::Serial* serialPort;
	CarPool* CarSource;
};
#endif //DATA_INTERFACE_H
