#include "pch.h"

#include "Analysis/AnalysisReturnCodes.h"
#include "Analysis/Analysis.h"
#include "CarPool.h"
#include "CarData.h"
#include "InterThreadComm.h"
#include "endpoint.h"
#include "sharedCache.h"

TEST(AnalysisParentTests, InitializeTest) {
	sharedCache<CarData *> boxCache;
	sharedCache<CarData *> carCache;
	endpoint<CarData *> updateQueue;
	std::string configFileName;
}