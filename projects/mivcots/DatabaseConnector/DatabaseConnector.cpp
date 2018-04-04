#include "DatabaseConnector.h"

DatabaseConnector::DatabaseConnector()
{

}

DatabaseConnector::~DatabaseConnector()
{
}

void DatabaseConnector::initCar(int carnum) {
	MYSQL mysql;
	mysql_init(&mysql);
	int tablefound = 0;
	char input = 'A';
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "initCar Failed to connect to database. I will now create the database: Error: " << mysql_error(&mysql) << std::endl;
		std::cout << "Would you like to create database:" << database << "Y/N" << std::endl;
		while (input != 'Y' || 'N') {
			std::cin >> input;
		}
		if (input == 'Y') {
			createDatabase(database);
		}
		else
			std::cout << "Database not created" << std::endl;
	}
	mysql_select_db(&mysql, database); //Used to test if it was able to select database. Will return 0 if worked.
	//std::cout << "initCar Connected to Database" << std::endl;
	createTable(carnum);
	mysql_close(&mysql);
}

void DatabaseConnector::createTable(int carnum) {
	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "createTable Failed to connect to database. Error: " << mysql_error(&mysql) << std::endl;
	}
	//std::cout << "createTable Connected to Database" << std::endl;
	std::string str1 = "CREATE TABLE car";
	std::string str2 = "";
	str2 = std::to_string(carnum);
	std::string str3 = " (UniqueID MEDIUMINT NOT NULL AUTO_INCREMENT,timestamp BIGINT,PRIMARY KEY (UniqueID));";// NOT NULL AUTO_INCREMENT PRIMARY KEY (UniqueID)  ADD Miliseconds Column 
	std::string NewCarTable = str1 + str2 + str3;
	const char* cstr = new char[NewCarTable.length() + 1];
	cstr = NewCarTable.c_str();
	int result = mysql_query(&mysql, cstr);
	if (result > 0) {
		std::cout << "Table Found for car" << carnum << std::endl;
	}
	else
		std::cout << "Table car" << carnum << " was not found and has been created" << std::endl;
	mysql_close(&mysql);
}

void DatabaseConnector::addDataAndColumnToTable(int carnum,long long datetime, std::string sensortype, double data) {
	MYSQL mysql;
	mysql_init(&mysql);
	int result = 0;
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "addDataAndColumnToTable Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}
	//std::cout << "addDataAndColumnToTable Connected to Database" << std::endl;
	//ADDS sensor column to table if not there
	std::string str1 = "ALTER TABLE car";
	std::string str2 = "";
	str2 = std::to_string(carnum);
	std::string str3 = " ADD ";
	std::string str4 = sensortype;
	std::string str5 = " DOUBLE;";
	std::string NewCarTable = str1 + str2 + str3 + str4+ str5;
	std::cout << NewCarTable << std::endl;
	const char* cstr = new char[NewCarTable.length() + 1];
	cstr = NewCarTable.c_str();
	mysql_query(&mysql, cstr);
	//Store data with datetime
	str1 = "INSERT INTO car";
	str2 = "";
	str2 = std::to_string(carnum);
	str3 = " (timestamp, ";
	str4 = sensortype;
	str5 = ") VALUES (";
	std::string str6 = std::to_string(datetime);
	std::string str7 = ", ";
	std::string str8 = std::to_string(data);
	std::string str9 = ");";
	NewCarTable = str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8 + str9;
	std::cout << NewCarTable << std::endl;
	cstr = new char[NewCarTable.length() + 1];
	cstr = NewCarTable.c_str();
	result = mysql_query(&mysql, cstr);
	if (result == 1) {
		std::cout << "Error adding data to Database" << std::endl;
	}
	mysql_close(&mysql);
}

void DatabaseConnector::addNewColumn(int carnum, std::string columnName, std::string columnType) {
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int columnFound = 0;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "addNewColumn Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}
	//std::cout << "addNewColumn connected to database" << std::endl;
	std::string str1 = columnName;
	const char wild = NULL;
	result = mysql_list_tables(&mysql, wild);
	while (row = mysql_fetch_row(result)) {
		int endOfRow = 1;
		for (int i = 0; i < endOfRow; i++) {
			//printf("%s\t", row[i]);
			if (row[i] == str1) {
				columnFound = 1;
			}
		}
		//printf("\n");
		//std::cout << row[0] , '\t' ,row[1] , '\t' , row[2], '\n';
		//printf("%s\t%s\t%s\n", row[0], row[1], row[2]);
	}
	if (columnFound == 1) {
		std::string str1 = "ALTER TABLE car";
		std::string str2 = "";
		str2 = std::to_string(carnum);
		std::string str3 = " ADD ";
		std::string str4 = columnName;
		std::string str5 = " ";
		std::string str6 = columnType;
		std::string NewCarTable = str1 + str2 + str3 + str4 + str5 + str6;
		const char* cstr = new char[NewCarTable.length() + 1];
		cstr = NewCarTable.c_str();
		mysql_query(&mysql, cstr);
		std::cout << "Column created in table" << std::endl;
	}
	else
		std::cout << "Column is already in table" << std::endl;
	mysql_free_result(result);
	mysql_close(&mysql);
}

//Add Data to Table. EIther use Insert into or possibly update.
void DatabaseConnector::addDataToTable(int carnum, long long datetime, std::string columnName, double storedata) {
	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}

	std::cout << "addDataToTable connected to database" << std::endl;
	std::string str1 = "INSERT INTO car";
	std::string str2 = "";
	str2 = std::to_string(carnum);
	std::string str3 = " (timestamp, ";
	std::string str4 = columnName;//The type of sensor
	std::string str5 = ") VALUES (";
	std::string str6 = std::to_string(datetime);
	std::string str7 = ", ";
	std::string str8 = std::to_string(storedata);
	/*
	if (datatype == 's') {//s long, u = unsigned long, d = double. Gets data from CarData class
		long longdata = CarData::get(char* key, long* dest);
		str8 = std::to_string(longdata);
	}
	else if (datatype == 'u') {
		unsigned long ulongdata = CarData::get(char* key, unsigned long dest);
		str8 = std::to_string(ulongdata);
	}
	else if (datatype == 'd') {
		double doubledata = CarData::get(char* key, double dest);
		str8 = std::to_string(doubledata);
	}
	else
		std::cout << "Error with datatype for storing data. Line 119-133. Datatype is " + datatype << std::endl;
	*/
	std::string str9 = ");";
	std::string NewCarTable = str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8 + str9;
	const char* cstr = new char[NewCarTable.length() + 1];
	//std::cout << NewCarTable << std::endl;
	cstr = new char[NewCarTable.length() + 1];
	cstr = NewCarTable.c_str();
	if (mysql_query(&mysql, cstr)>0) {
		std::cout << "Error adding data to Database" << std::endl;
	}

	mysql_close(&mysql);
}

void DatabaseConnector::getData(int carnum, long long minValue, long long maxValue) {//get data for all columns. if timerange not specified then give everything. Want to be able to refine by timestamp
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}
	std::cout << "getData connected to database" << std::endl;
	//SELECT * FROM car# WHERE timestamp > # AND timestamp < #; 
	std::string str1 = "SELECT * FROM car";
	std::string str2 = std::to_string(carnum);
	std::string str3 = " WHERE timestamp > ";
	std::string str4 = std::to_string(minValue);
	std::string str5 = " AND timestamp < ";
	std::string str6 = std::to_string(maxValue);
	std::string str7 = ";";
	std::string finalString = str1 + str2 + str3 + str4 + str5 + str6 + str7;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	mysql_query(&mysql, cstr);
	result = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(result)) {
		int endOfRow = 3;
		for (int i = 0; i < endOfRow; i++) {
			printf("%s\t", row[i]);
		}
		printf("\n");
	}
	mysql_free_result(result);
	mysql_close(&mysql);
}

void DatabaseConnector::dropTable(int carnum) {
	MYSQL mysql;;
	int tablefound = 0;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}
	std::cout << "dropTable connected to database" << std::endl;
	std::string str1 = "DROP TABLE car";
	std::string str2 = "";
	str2 = std::to_string(carnum);
	std::string finalString = str1 + str2;
	std::cout << finalString << std::endl;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	tablefound = mysql_query(&mysql, cstr);
	if(tablefound == 0)
		std::cout << "Table car" << carnum <<  " was dropped. I hope you meant to do this!" << std::endl;
	else	
		std::cout << "ERROR: Table car" << carnum << " was NOT dropped!" << std::endl;
	
	mysql_close(&mysql);
}

//delete row of data from table   Delete a row(s) from a table.		DELETE from [table name] where [field name] = 'whatever';
void DatabaseConnector::dropRowFromTable(int carnum, long long timestamp) {
	MYSQL mysql;
	int tablefound = 0;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}
	std::cout << "getRowFromTable connected to database" << std::endl;
	std::string str1 = "DELETE FROM car";
	std::string str2 = std::to_string(carnum);
	std::string str3 = " WHERE timestamp = ";
	std::string str4 = std::to_string(timestamp);
	std::string str5 = ";";
	std::string finalString = str1 + str2 + str3 + str4 + str5;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	tablefound = mysql_query(&mysql, cstr);
	if(tablefound == 0)
		std::cout << "Timestamp " << timestamp << " was deleted from Table car"<< carnum << ". I hope you meant to do this!" << std::endl;
	else
		std::cout << "ERROR: Timestamp " << timestamp << " was NOT deleted from Table car" << carnum << std::endl;

	mysql_close(&mysql);
}

//delete column from table  Delete a column.	alter table [table name] drop column [column name];
void DatabaseConnector::dropColumn(int carnum,std::string columnName) {
	MYSQL mysql;
	int columnfound = 0;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, host, user, passwd, database, 0, NULL, 0))
	{
		std::cout << "Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}
	std::cout << "dropColumn connected to database" << std::endl;
	std::string str1 = "ALTER TABLE car";
	std::string str2 = std::to_string(carnum);
	std::string str3 = " DROP COLUMN ";
	std::string str4 = columnName;
	std::string str5 = ";";
	std::string finalString = str1 + str2 + str3 + str4 + str5;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	columnfound = mysql_query(&mysql, cstr);//returns 0 if success. 1 if failed
	if(columnfound == 0)
		std::cout << "Column " << columnName << " was deleted from Table car" << carnum << ". I hope you meant to do this!" << std::endl;
	else
		std::cout << "ERROR: Column " << columnName << " was NOT deleted from Table car" << carnum << std::endl;
	
	mysql_close(&mysql);
}

//possibly create database Create a database on the sql server.		create database [databasename];
void DatabaseConnector::createDatabase(std::string databaseName) {
	MYSQL mysql;
	mysql_init(&mysql);
	int result = 0;
	if (!mysql_real_connect(&mysql, host, user, passwd, NULL, 0, NULL, 0))
	{
		std::cout << "Failed to connect to database. Please use cmd to create database first: Error: " << mysql_error(&mysql) << std::endl;
	}
	std::cout << "createDatabase connected to database" << std::endl;
	std::string str1 = "CREATE DATABASE ";
	std::string str2 = databaseName;
	std::string finalString = str1 + str2;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	result = mysql_query(&mysql, cstr);
	if (result == 0)
		std::cout << "Databse: " << databaseName << " was created" << std::endl;
	else
		std::cout << "ERROR: Databse " << databaseName << " was NOT created." << std::endl;
	mysql_close(&mysql);
}


//update table UPDATE [table name] SET Select_priv = 'Y',Insert_priv = 'Y',Update_priv = 'Y' where [field name] = 'user';
=======
>>>>>>> 828c8ed160968088538760bb592824bb79bd27da
