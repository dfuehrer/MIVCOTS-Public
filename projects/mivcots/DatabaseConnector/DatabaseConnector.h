#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H
#include <mysql.h>
#include <wx/wx.h>
#include <atomic>
#include <iostream>
#include <future>
#include <thread>
#include <sstream>
#include <string.h>
#include <chrono>
#include "CacheBank.h"
#include "CarPool.h"
#include "InterThreadComm.h"
#include "key_defines.h"
#include "error_codes.h"
#include "Utilities.h"

typedef struct databaseInfo_t {
	long carID;
	long startTime;
	long startDate;
	long endTime;
	long endDate;
} databaseInfo;

class DatabaseConnector
{
public:
	DatabaseConnector();
	~DatabaseConnector();

	int initialize(endpoint<CarData*>* _dataQ, endpoint<CarData*>* _boxDataQ, 
				   endpoint<CarData*>* _analysisQ, CarPool* _carSource, 
		           CacheBank* _outputCache);

	int start();
	int stop();

	int AvailablePlaybackData(std::vector<databaseInfo>* availableInfo);
	int AddData(CarData *receivedData); 
	int InitializeDatabase();
	//int GetData(long carnum, long minDateValue, long maxDateValue, long minTimeValue, long maxTimeValue);
	int UpdateData(CarData *updateData);
	int shutdown();
	int dropTable(long carnum);
	int dropRowFromTable(long carnum, long long timestamp);
	int dropColumn(long carnum, std::string columnName);

	int startPlayback(databaseInfo playbackRequest, double timeFactor);
	
private:

	std::atomic<bool> isRunning;
	std::thread databaseThread;
	void runDatabaseThread();
	bool databaseConnected;
	endpoint<CarData*>* dataQ;
	endpoint<CarData*>* boxDataQ;
	endpoint<CarData*>* analysisQ;
	CarPool* carSource;
	CacheBank* outputCache;

	MYSQL mysql;

	char host[32] = "localhost";
	char user[32] = "root";
	char passwd[32] = "Telemetry";
	char database[64] = "mivcots";
	int knownCarTables[128];
	std::string columnDataTypes[128];//[carNum][columnNum]
	int endpointOfColumnTypeList;

	int initDB();
	int addNewColumn(CarData *receivedData);
	int addDataToTable(CarData *receivedData);
	int tableUpdate(CarData *updateData);
	
	int createDatabase();
	int createTable(CarData *receivedData);
	int closeConnection();
	int selectDatabase();
	int getColumnTypes(long carnum);

	// TODO: don't keep all the threads ever
	std::vector<std::atomic<bool>*> playbackThreadStatus;
	std::vector<std::thread> playbackThreads;
	int getDataTimestamp(std::atomic<bool>* status, long carnum, long minDateValue, long maxDateValue, long minTimeValue, long maxTimeValue, double timeFactor);
};
#endif //DATABASE_CONNECTOR_H
