#pragma once


#include <atomic>
#include <iostream>
#include <thread>
#include <wx/wx.h>
#include <wx/app.h>
#include "CarData.h"
#include "endpoint.h"
#include "serial/serial.h"

#define DELIMITER ","

class DataInterface
{
public:
	DataInterface();
	~DataInterface();

	int initialize(std::string portName, long int baud, endpoint<CarData*>* _outputQ);
	void start();
	void stop();

protected:
	std::atomic<bool> isRunning;
	std::thread serialThread;

	endpoint<CarData*>* outputQ;

	void runSerialThread();

	int parseString(std::string toParse, CarData** parsed);

	serial::Serial* serialPort;
};

