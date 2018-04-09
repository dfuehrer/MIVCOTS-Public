#include "Analysis.h"
#include <wx/log.h>
#include "AnalysisReturnCodes.h"



AnalysisParent::AnalysisParent()
{
}


AnalysisParent::~AnalysisParent()
{
}



int AnalysisParent::init(
	sharedCache<CarData*> * boxCache,
	sharedCache<CarData*> * carCache,
	endpoint<CarData*> * updateQueue,
	CarPool * carPool,
	std::string configFileName
)
{
	int returnCode = 0;
	if (boxCache != nullptr) {
		this->boxCache = boxCache;
	}
	else {
		wxLogError("Analysis Parent - Init Function: boxCache Pointer is Null");
		returnCode |= ERR_BOX_CACHE_PTR_IS_NULL;
	}
	if (carCache != nullptr) {
		this->carCache = carCache;
	}
	else {
		wxLogError("Analysis Parent - Init Function: carCache Pointer is Null");
		returnCode |= ERR_CAR_CACHE_PTR_IS_NULL;
		return returnCode;
	}
	if (updateQueue != nullptr) {
		this->updateQueue = updateQueue;
	}
	else {
		wxLogError("Analysis Parent - Init Function: updateQueue Pointer is Null");
		returnCode |= ERR_UPDATE_QUEUE_PTR_IS_NULL;
		return returnCode;
	}
	if (carPool != nullptr) {
		this->carPool = carPool;
	}
	else {
		wxLogError("Analysis Parent - Init Function: carPool Pointer is Null");
		returnCode |= ERR_CAR_CACHE_PTR_IS_NULL;
		return returnCode;
	}
	if (!(configFileName.empty())) {
		this->configFileName = configFileName;
	}
	else {
		wxLogError("Analysis Parent - Init Function: Config File Name is Empty");
		returnCode |= WARN_CONFIG_FILENAME_IS_EMPTY;
	}


	return returnCode;
}

int AnalysisParent::start()
{
	return 0;
}

int AnalysisParent::stop()
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