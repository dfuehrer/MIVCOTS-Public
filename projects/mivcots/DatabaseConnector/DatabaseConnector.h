#pragma once
#include <mysql.h>
#include <wx/wx.h>
#include <atomic>
#include <iostream>
#include <thread>
#include <sstream>
#include <string.h>
#include "CarPool.h"
#include "InterThreadComm.h"


class DatabaseConnector
{
public:
	DatabaseConnector();
	~DatabaseConnector();

	std::string carRowData[6000][6000];

	//int AddData(int carnum, std::string sensortype, std::string sensorvar, long long datetime, double data, endpoint <CarData*, CarData* > inputqadd);
	int AddData(int carnum, std::string sensortype, std::string sensorvar, long long datetime, double data);
	int InitializeDatabase(std::string database);
	//int GetData(int carnum, long long minValue, long long maxValue, endpoint <CarData*, CarData* > outputq );
	int GetData(int carnum, long long minValue, long long maxValue);
	//int UpdateData(int carnum, int uniqueID, std::string columnName, double updatedValue, endpoint <CarData*, CarData* > inputq);
	int UpdateData(int carnum, int uniqueID, std::string columnName, double updatedValue);
	int shutdown();
	int dropTable(int carnum);
	int dropRowFromTable(int carnum, long long timestamp);
	int dropColumn(int carnum, std::string columnName);

private:

	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	CarPool* CarSource;

	char host[32] = "localhost";
	char user[32] = "root";
	char passwd[32] = "somethingsecure";
	char database[64] = "mivcots";
	int knownCarTables[128];
	std::string columnDataTypes[128][128];//[carNum][columnNum]

	int initDB(CarPool* _CarSource);
	int addNewColumn(int carnum, std::string columnName, std::string columnType);
	//int addDataToTable(int carnum, long long datetime, std::string columnName, double storedata, endpoint <CarData*, CarData* > inputqadd);
	int addDataToTable(int carnum, long long datetime, std::string columnName, double storedata);
	//int tableUpdate(int carnum, int uniqueID, std::string columnName, double undatedValue, endpoint <CarData*, CarData* > inputqupdate);
	int tableUpdate(int carnum, int uniqueID, std::string columnName, double undatedValue);
	//int getDataTimestamp(int carnum, long long minValue, long long maxValue, endpoint <CarData*, CarData* > outputq);
	int getDataTimestamp(int carnum, long long minValue, long long maxValue);
	int createDatabase(std::string databaseName);
	int createTable(int carnum);
	int closeConnection();
	int freeResult();
	int selectDatabase(std::string databaseName);
	int getColumnTypes(int carnum);

};