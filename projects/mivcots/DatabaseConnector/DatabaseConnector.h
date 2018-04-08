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

	int AddData(int carnum, std::string sensortype, std::string sensorvar, long long datetime, double data);
	int InitializeDatabase(std::string database);

private:

	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	CarPool* CarSource;

	char host[32] = "localhost";
	char user[32] = "root";
	char passwd[32] = "somethingsecure";
	char database[64] = "mivcots";

	int initDB(CarPool* _CarSource);
	int addNewColumn(int carnum, std::string columnName, std::string columnType);
	int addDataToTable(int carnum, long long datetime, std::string columnName, double storedata);
	int getDataTimestamp(int carnum, long long minValue, long long maxValue, endpoint <CarData*, CarData* > outputq);
	int dropTable(int carnum);
	int dropRowFromTable(int carnum, long long timestamp);
	int dropColumn(int carnum, std::string columnName);
	int createDatabase(std::string databaseName);
	int createTable(int carnum);
	int shutdown();
	int closeConnection();
	int freeResult();
	int selectDatabase(std::string databaseName);
	int tableUpdate(int carnum, int uniqueID, std::string columnName, double undatedValue);

};