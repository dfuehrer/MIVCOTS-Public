#ifndef ANOMALY_DETECTION_H
#define ANOMALY_DETECTION_H
#include "AnalysisChild.h"
class AnomalyDetection :
	public AnalysisChild
{
public:
	AnomalyDetection();
	~AnomalyDetection();


private:
	bool speedAnomalyDetected;
	bool upsideDownAnomalyDetected;

	int upsideDownCounter;

	int setup();
	int loop();

	
};

#endif //ANOMALY_DETECTION_H
