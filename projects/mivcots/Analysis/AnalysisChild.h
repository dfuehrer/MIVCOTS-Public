#include "CarData\CarData.h"
#include "CarPool\CarPool.h"
#include "InterThreadComm\sharedCache.h"
#include "InterThreadComm\endpoint.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "error_codes.h"

class AnalysisChild
{
public:
	AnalysisChild();
	virtual ~AnalysisChild();
	virtual int init(
		sharedCache<CarData *> * boxCache,
		sharedCache<CarData *> * carCache,
		endpoint<CarData *> * updateQueue,
		CarPool * carPool,
		std::mutex * analysisFinishedCounterMutex,
		std::atomic<int> * analysisFinishedCounterInt,
		std::mutex * analysisStepMutex,
		std::condition_variable * analysisStepConditionVariable
	);
	int start();
	int stop();
	int runThread();



protected:
	sharedCache<CarData*> * boxCache;
	sharedCache<CarData*> * carCache;
	endpoint<CarData*> * updateQueue;
	CarPool * carPool;
	std::thread analysisThread;
	std::atomic<bool> isRunning;

	std::mutex * analysisStepMutex;
	std::condition_variable * analysisStepConditionVariable;

	std::mutex * analysisFinishedCounterMutex;
	std::atomic<int> * analysisFinishedCounterInt;

	virtual int setup();
	virtual int loop();

};
