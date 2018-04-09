#include "AnalysisChild.h"



AnalysisChild::AnalysisChild()
{
}


AnalysisChild::~AnalysisChild()
{
}

int AnalysisChild::init(
	sharedCache<CarData*> * boxCache, 
	sharedCache<CarData*> * carCache, 
	endpoint<CarData*> * updateQueue, 
	CarPool * carPool
)
{
	return 0;
}

int AnalysisChild::start()
{
	return 0;
}

int AnalysisChild::stop()
{
	return 0;
}


