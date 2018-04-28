#include "CarData\CarData.h"
#include "CarPool\CarPool.h"
#include "InterThreadComm\sharedCache.h"
#include "InterThreadComm\endpoint.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "error_codes.h"

// Stores all of the threading-related pointers for analysis children
typedef struct AnalysisSyncVars_st {
	std::mutex * analysisFinishedCounterMutex;
	std::atomic<int> * analysisFinishedCounterInt;
	std::mutex * analysisStepMutex;
	std::condition_variable * analysisStepConditionVariable;
	std::atomic<bool>* analysisStepInt;
} AnalysisSyncVars;

class AnalysisChild
{
public:
	AnalysisChild();
	virtual ~AnalysisChild();
	virtual int init(
		sharedCache<CarData *> * boxCache,
		sharedCache<CarData *> * carCache,
		lockedQueue<CarData*> * updateQueue,
		CarPool * carPool,
		AnalysisSyncVars * analysisSyncVars
	);
	int start();
	int stop();
	int runThread();



protected:
	sharedCache<CarData*> * boxCache;
	sharedCache<CarData*> * carCache;
	lockedQueue<CarData*> * updateQueue;
	CarPool * carPool;
	std::thread analysisThread;
	std::atomic<bool> isRunning;

	AnalysisSyncVars * analysisSyncVars;

	virtual int setup();
	virtual int loop();

};
