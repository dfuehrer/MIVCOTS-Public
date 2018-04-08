#pragma once


#include <vector>
#include "InterThreadComm\endpoint.h"
#include "CarData.h"


class Analysis
{
public:
	Analysis();
	~Analysis();
	
	// Communication Interface back to MIVCOTS
	//endpoint<CarData, CarData> * interface;
	std::vector<CarData*> carData;

	std::vector<double> DataPoints;//most recent point will be at the end
	int ControlChart(int GraphPoints);
	
	//int Warning;
	//double AvgData;
	//double UpperControlLimit;
	//double LowerControlLimit;
};

