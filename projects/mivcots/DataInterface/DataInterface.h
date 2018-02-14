#pragma once

#include <atomic>
#include <iostream>
#include <thread>

#include "CarData.h"
#include "serial/serial.h"

class DataInterface
{
public:
	DataInterface();
	~DataInterface();

	void initialize(std::string portName, long int baud);
	void start();
	void stop();

protected:
	std::atomic<bool> isRunning;
	std::thread serialThread;

	void runSerialThread();

	CarData parseString(std::string toParse);

	serial::Serial* dataSource;
};

