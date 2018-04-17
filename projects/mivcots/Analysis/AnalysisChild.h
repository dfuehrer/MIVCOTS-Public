#include "CarData\CarData.h"
#include "CarPool\CarPool.h"
#include "InterThreadComm\sharedCache.h"
#include "InterThreadComm\endpoint.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class AnalysisChild
{
public:
	AnalysisChild();
	virtual ~AnalysisChild();
	virtual int init(
		sharedCache<CarData *> * boxCache, 
		sharedCache<CarData *> * carCache, 
		endpoint<CarData *> * updateQueue, 
		CarPool * carPool
	);
	virtual int start();
	virtual int stop();
	virtual int runThread();



protected:
	sharedCache<CarData*> * boxCache;
	sharedCache<CarData*> * carCache;
	endpoint<CarData*> * updateQueue;
	CarPool * carPool;
	std::thread analysisThread;
	std::atomic<bool> isRunning;

};
