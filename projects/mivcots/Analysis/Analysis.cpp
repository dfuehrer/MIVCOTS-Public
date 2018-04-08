#include "Analysis.h"




Analysis::Analysis()
{
}


Analysis::~Analysis()
{
}

int Analysis::init(sharedCache<CarData*> boxCache, sharedCache<CarData*> carCache, endpoint<CarData*> updateQueue, CarPool * carPool)
{
	return 0;
}

//int Analysis::ControlChart(int GraphPoints) {
//	int i = 0;
//	double DataPointSum = 0;
//	double StandDeviation = 0;
//	double TopEqn = 0;
//	double UpperControlLimit = 0;
//	double LowerControlLimit = 0;
//	double AvgData = 0;
//	double Warning = 0;
//	//begin Initializaatshion
//	for (i = 0; i < GraphPoints - 1; i++) {
//		DataPointSum = DataPoints[i] + DataPointSum;
//	}
//	AvgData = DataPointSum / (GraphPoints - 1);
//	for (i = 0; i < GraphPoints - 1; i++) {
//		TopEqn = pow(DataPoints[i] - DataPointSum, 2)+ TopEqn;
//	}
//	StandDeviation = pow(TopEqn / (GraphPoints - 1), 0.5);
//	UpperControlLimit = DataPointSum + StandDeviation;
//	LowerControlLimit = DataPointSum - StandDeviation;
//
//	if (DataPoints[GraphPoints] >= UpperControlLimit) {
//		Warning = 1;
//	}
//	if (DataPoints[GraphPoints] <= LowerControlLimit) {
//		Warning = 1;
//	}
//	else
//		Warning = 0; 
//	return 0;
//}