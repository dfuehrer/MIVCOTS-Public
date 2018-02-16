#pragma once

#include <vector>

class Analysis
{
public:
	Analysis();
	~Analysis();
	
	std::vector<double> DataPoints;//most recent point will be at the end
	int ControlChart(int GraphPoints);
	
	//int Warning;
	//double AvgData;
	//double UpperControlLimit;
	//double LowerControlLimit;
};

