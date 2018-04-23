#pragma once
#include <mysql.h>
#include <wx/wx.h>
#include <atomic>
#include <iostream>
#include <thread>
#include <sstream>
#include <string.h>
#include "CacheBank.h"
#include "CarPool.h"
#include "InterThreadComm.h"
#include "key_defines.h"
#include "error_codes.h"

class DatabaseConnector
{
public:
	DatabaseConnector();
	~DatabaseConnector();

	int initialize(endpoint<CarData*>* _dataQ, endpoint<CarData*>* _boxDataQ, CarPool* _carSource, CacheBank* _outputCache);

	int start();
	int stop();

	std::string carRowData[100];

	int AddData(CarData *receivedData); 
	int InitializeDatabase();
	int GetData(long carnum, long minDateValue, long maxDateValue, long minTimeValue, long maxTimeValue);
	int UpdateData(long carnum, int uniqueID, std::string columnName, double updatedValue);
	int shutdown();
	int dropTable(long carnum);
	int dropRowFromTable(long carnum, long long timestamp);
	int dropColumn(long carnum, std::string columnName);

private:

	std::atomic<bool> isRunning;
	std::thread databaseThread;
	void runDatabaseThread();

	endpoint<CarData*>* dataQ;
	endpoint<CarData*>* boxDataQ;
	CarPool* carSource;
	CacheBank* outputCache;

	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	CarPool* CarSource;

	char host[32] = "localhost";
	char user[32] = "root";
	char passwd[32] = "Telemetry";
	char database[64] = "mivcots";
	int knownCarTables[128];
	std::string columnDataTypes[128];//[carNum][columnNum]
	int endpointOfColumnTypeList;

	int initDB(CarPool* _CarSource);
	int addNewColumn(CarData *receivedData);
	int addDataToTable(CarData *receivedData);
	int tableUpdate(long carnum, int uniqueID, std::string columnName, double undatedValue);
	int getDataTimestamp(long carnum, long minDateValue, long maxDateValue, long minTimeValue, long maxTimeValue);
	int createDatabase();
	int createTable(CarData *receivedData);
	int closeConnection();
	int freeResult();
	int selectDatabase();
	int getColumnTypes(long carnum);

};