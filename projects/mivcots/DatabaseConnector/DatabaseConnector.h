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
	void initCar(int carnum);
	void addDataAndColumnToTable(int carnum, long long datetime, std::string sensor, double data);
	void addNewColumn(int carnum, std::string columnName, std::string columnType);
	//void addDataToTable(int carnum, long long datetime, std::string columnName, char datatype, char* datakey);
	void addDataToTable(int carnum, long long datetime, std::string columnName, double storedata);
	void getData(int carnum, long long minValue, long long maxValue);
	void dropTable(int carnum);
	void dropRowFromTable(int carnum, long long timestamp);
	void dropColumn(int carnum, std::string columnName);
	void createDatabase(std::string databaseName);
	void createTable(int carnum);
};