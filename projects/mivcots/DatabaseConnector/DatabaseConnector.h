#pragma once
#include <mysql.h>
#include <atomic>
#include <iostream>
#include <thread>
#include <sstream>
#include <string.h>
#include "CarData.h"


class DatabaseConnector
{
public:
	DatabaseConnector();
	~DatabaseConnector();
	char host[32] = "localhost";
	char user[32] = "root";
	char passwd[32] = "somethingsecure";
	char database[64] = "mivcots";
	//convert all void functions to int so it can return 0 for success and 1 for fail for unit testing
	int initDB();
	void addDataAndColumnToTable(int carnum, long long datetime, std::string sensor, double data);
	int addNewColumn(int carnum, std::string columnName, std::string columnType);
	//void addDataToTable(int carnum, long long datetime, std::string columnName, char datatype, char* datakey);
	int addDataToTable(int carnum, long long datetime, std::string columnName, double storedata);
	int getDataTimestamp(int carnum, long long minValue, long long maxValue);
	int dropTable(int carnum);
	int dropRowFromTable(int carnum, long long timestamp);
	int dropColumn(int carnum, std::string columnName);
	int createDatabase(std::string databaseName);
	int createTable(int carnum);
	int shutdown();
	int closeConnection();
	int freeResult();
	int tableUpdate(int carnum, int uniqueID, std::string columnName, long undatedValue);
private:
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
};