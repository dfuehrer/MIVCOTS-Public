#pragma once


#include <atomic>
#include <iostream>
#include <thread>
#include <wx/wx.h>
#include "CarData.h"
#include "CarPool.h"
#include "endpoint.h"
#include "serial/serial.h"

#define DELIMITER ","
#define ENDOFMSG "!"

class DataInterface
{
public:
	DataInterface();
	~DataInterface();

	int initialize(std::string portName, long int baud, endpoint<CarData*>* _outputQ, CarPool* _CarSource);
	void start();
	void stop();

protected:
	std::atomic<bool> isRunning;
	std::thread serialThread;

	endpoint<CarData*>* outputQ;

	void runSerialThread();

	int parseString(std::string toParse, CarData** parsed);

	serial::Serial* serialPort;
	CarPool* CarSource;
};

