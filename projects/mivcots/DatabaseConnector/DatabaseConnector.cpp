#include "DatabaseConnector.h"

DatabaseConnector::DatabaseConnector()
{

}

DatabaseConnector::~DatabaseConnector()
{
}

//Work Flow 
//1. initDB()
//n-1. DB manipulation
//n. shutdownDB();

int DatabaseConnector::initDB(CarPool* _CarSource) {
	mysql_init(&mysql);
	CarSource = _CarSource;
	int success = 0;
	char input = 'A';
	if (!mysql_real_connect(&mysql, host, user, passwd, NULL, 0, NULL, 0))
	{
		std::cout << "initCar Failed to connect to connect to MySQL Server: Error: " << mysql_error(&mysql) << std::endl;
		/*std::cout << "Would you like to create database:" << database << " Y/N" << std::endl;
		while (input != 'Y' || 'N') {
			std::cin >> input;
		}
		if (input == 'Y') {
			createDatabase(database);
		}
		else
			std::cout << "Database not created" << std::endl;*/
	}
	//success = mysql_select_db(&mysql, database); //Used to test if it was able to select database. Will return 0 if worked.
	if (success == 0)
		return 0;
	else
		return 1;
}

int DatabaseConnector::createTable(int carnum) {
	std::string str1 = "CREATE TABLE car";
	std::string str2 = "";
	str2 = std::to_string(carnum);
	std::string str3 = " (UniqueID MEDIUMINT NOT NULL AUTO_INCREMENT,timestamp BIGINT,PRIMARY KEY (UniqueID));";// NOT NULL AUTO_INCREMENT PRIMARY KEY (UniqueID)  ADD Miliseconds Column 
	std::string NewCarTable = str1 + str2 + str3;
	const char* cstr = new char[NewCarTable.length() + 1];
	cstr = NewCarTable.c_str();
	int result = mysql_query(&mysql, cstr);
	if (result > 0) {
		//std::cout << "Table Found for car" << carnum << std::endl;
		return 0;
	}
	else {
		//std::cout << "Table car" << carnum << " was not found and has been created" << std::endl;
		return 1;
	}
}

/*
void DatabaseConnector::addDataAndColumnToTable(int carnum,long long datetime, std::string sensortype, double data) {
	int result = 0;
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
}
*/

int DatabaseConnector::addNewColumn(int carnum, std::string columnName, std::string columnType) {
	int columnFound = 0;
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
		mysql_free_result(result);
		//std::cout << "Column created in table" << std::endl;
		return 0;
	}
	else {
		mysql_free_result(result);
		//std::cout << "Column is already in table" << std::endl;
		return 1;
	}
	
}

//Add Data to Table. EIther use Insert into or possibly update.
int DatabaseConnector::addDataToTable(int carnum, long long datetime, std::string columnName, double storedata) {
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
	if (mysql_query(&mysql, cstr) > 0) {
		std::cout << "Error adding data to Database" << std::endl;
		return 1;
	}
	else
		return 0;
}

int DatabaseConnector::getDataTimestamp(int carnum, long long minValue, long long maxValue, endpoint <CarData*, CarData* > outputq) {//get data for all columns. if timerange not specified then give everything. Want to be able to refine by timestamp
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
	while (row = mysql_fetch_row(result)) {//put into cardata object
		int endOfRow = 3;
		for (int i = 0; i < endOfRow; i++) {
			printf("%s\t", row[i]);
		}
		printf("\n");

	}
	return 0;
}

int DatabaseConnector::dropTable(int carnum) {
	int pass = 0;
	std::string str1 = "DROP TABLE car";
	std::string str2 = "";
	str2 = std::to_string(carnum);
	std::string finalString = str1 + str2;
	std::cout << finalString << std::endl;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {
		//std::cout << "Table car" << carnum << " was dropped. I hope you meant to do this!" << std::endl;
		return 0;
	}
	else {
		std::cout << "ERROR: Table car" << carnum << " was NOT dropped!" << std::endl;
		return 1;
	}
}

//delete row of data from table   Delete a row(s) from a table.		DELETE from [table name] where [field name] = 'whatever';
int DatabaseConnector::dropRowFromTable(int carnum, long long timestamp) {
	int pass = 0;
	std::string str1 = "DELETE FROM car";
	std::string str2 = std::to_string(carnum);
	std::string str3 = " WHERE timestamp = ";
	std::string str4 = std::to_string(timestamp);
	std::string str5 = ";";
	std::string finalString = str1 + str2 + str3 + str4 + str5;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {
		//std::cout << "Timestamp " << timestamp << " was deleted from Table car" << carnum << ". I hope you meant to do this!" << std::endl;
		return 0;
	}
	else {
		std::cout << "ERROR: Timestamp " << timestamp << " was NOT deleted from Table car" << carnum << std::endl;
		return 1;
	}
}

//delete column from table  Delete a column.	alter table [table name] drop column [column name];
int DatabaseConnector::dropColumn(int carnum,std::string columnName) {
	int pass = 0;
	std::string str1 = "ALTER TABLE car";
	std::string str2 = std::to_string(carnum);
	std::string str3 = " DROP COLUMN ";
	std::string str4 = columnName;
	std::string str5 = ";";
	std::string finalString = str1 + str2 + str3 + str4 + str5;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	pass = mysql_query(&mysql, cstr);//returns 0 if success. 1 if failed
	if (pass == 0) {
		//std::cout << "Column " << columnName << " was deleted from Table car" << carnum << ". I hope you meant to do this!" << std::endl;
		return 0;
	}
	else {
		std::cout << "ERROR: Column " << columnName << " was NOT deleted from Table car" << carnum << std::endl;
		return 1;
	}
}

//possibly create database Create a database on the sql server.		create database [databasename];
int DatabaseConnector::createDatabase(std::string databaseName) {
	int pass = 0;
	std::string str1 = "CREATE DATABASE ";
	std::string str2 = databaseName;
	std::string finalString = str1 + str2;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {
		//std::cout << "Databse: " << databaseName << " was created" << std::endl;
		return 0;
	}
	else {
		std::cout << "ERROR: Databse " << databaseName << " was NOT created." << std::endl;
		return 1;
	}
}

//shutdown mysql_shutdown(&mysql)
int DatabaseConnector::shutdown() {
	int pass = 0;
	pass = mysql_query(&mysql, "SHUTDOWN");
	closeConnection();//calls the close connection function
	if (pass == 0) {
		std::cout << "Shutdown Successful" << std::endl;
		return 0;
	}
	else {
		std::cout << "Shutdown Failed" << std::endl;
		return 1;
	}
}

//update table UPDATE [table name] SET Select_priv = 'Y',Insert_priv = 'Y',Update_priv = 'Y' where [field name] = 'user';
int DatabaseConnector::tableUpdate(int carnum, int uniqueID,std::string columnName, long updatedValue) {//can also use timestamp instead of uniqueID. Also can add multiple column update.
	int pass = 0;
	std::string str1 = "UPDATE car";
	std::string str2 = std::to_string(carnum);
	std::string str3 = " SET ";
	std::string str4 = columnName;
	std::string str5 = " = '";
	std::string str6 = std::to_string(updatedValue);
	std::string str7 = "' WHERE UniqueID = ";
	std::string str8 = std::to_string(uniqueID);
	std::string finalString = str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {
		//std::cout << "Table was updated" << std::endl;
		return 0;
	}
	else {
		//std::cout << "ERROR: Table was NOT created." << std::endl;
		return 1;
	}
}

//Close connection		 mysql_close(&mysql)
int DatabaseConnector::closeConnection() {
	mysql_close(&mysql);
	//std::cout << "Connection Closed" << std::endl;
	return 0;
}

//free result

int DatabaseConnector::freeResult() {
	mysql_free_result(result);
	return 0;
}