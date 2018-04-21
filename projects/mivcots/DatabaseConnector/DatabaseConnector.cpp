#include "DatabaseConnector.h"

DatabaseConnector::DatabaseConnector()
{

}

DatabaseConnector::~DatabaseConnector()
{
}

int DatabaseConnector::initialize(endpoint<CarData*>* _dataQ, endpoint<CarData*>* _boxDataQ, CarPool * _carSource, CacheBank * _outputCache)
{
	dataQ = _dataQ;
	boxDataQ = _boxDataQ;
	carSource = _carSource;
	outputCache = _outputCache;
	return SUCCESS;
}

int DatabaseConnector::start()
{
	isRunning.store(true, std::memory_order_relaxed);
	databaseThread = std::thread(&DatabaseConnector::runDatabaseThread, this);

	if (databaseThread.joinable()) {
		return SUCCESS;
	}
	else {
		return ERR_INITERR;
	}
}

int DatabaseConnector::stop()
{
	isRunning.store(false, std::memory_order_relaxed);
	if (databaseThread.joinable()) {
		databaseThread.join();
	}
	return SUCCESS;
}

//Work Flow 
//1. initDB()
//n-1. DB manipulation
//n. shutdownDB();


//////////////////////////////////////////////////////////////////////////Private Functions///////////////////////////////////////////////////////////////////////////

int DatabaseConnector::initDB(CarPool* _CarSource) {
	mysql_init(&mysql);
	CarSource = _CarSource;
	int success = 0;
	if (!mysql_real_connect(&mysql, host, user, passwd, NULL, 0, NULL, 0)){ // if failed
		return mysql_errno(&mysql);//Failed
	}
	else {//Success
		return 0;//Pass
	}
}

int DatabaseConnector::createTable(CarData *receivedData) {
	int pass = 0;
	int errorNum = NULL;
	long carnum;
	std::string str1 = "CREATE TABLE IF NOT EXISTS car";
	std::string str2 = "";
	str2 = std::to_string(receivedData->get(ID,&carnum));
	std::string str3 = "(UniqueID MEDIUMINT NOT NULL AUTO_INCREMENT,Date LONG,Time LONG,PRIMARY KEY (UniqueID));";// NOT NULL AUTO_INCREMENT PRIMARY KEY (UniqueID)  ADD Miliseconds Column 
	std::string NewCarTable = str1 + str2 + str3;
	const char* cstr = new char[NewCarTable.length() + 1];
	cstr = NewCarTable.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {//Success
		return 0;
	}
	else {//Failed
		return mysql_errno(&mysql);//Failed
	}
}

int DatabaseConnector::addNewColumn(CarData *receivedData) {
	int columnFound = 0;
	long carIDNum;
	std::map<std::string, std::string> ::iterator iter;

	//std::string str1 = columnName;
	//const char wild = NULL;
	//result = mysql_list_tables(&mysql, wild);
	//while (row = mysql_fetch_row(result)) {
	//	int endOfRow = 1;
	//	for (int i = 0; i < endOfRow; i++) {
	//		if (row[i] == str1) {
	//			columnFound = 1;
	//		}
	//	}
	//}
	for (iter = keyMap.begin();iter !=keyMap.end(); iter++) {
		std::string str1 = "ALTER TABLE car";
		std::string str2 = "";
		str2 = std::to_string(receivedData->get(ID,&carIDNum));
		std::string str3 = " ADD ";
		std::string str4 = iter->second;//columnName
		std::string str5 = " ";
		std::string str6 = "LONG";//columnType
		std::string NewCarTable = str1 + str2 + str3 + str4 + str5 + str6;
		const char* cstr = new char[NewCarTable.length() + 1];
		cstr = NewCarTable.c_str();
		mysql_query(&mysql, cstr);
		mysql_free_result(result);
	}
	return 0;	
}

//Add Data to Table. EIther use Insert into or possibly update.
//int DatabaseConnector::addDataToTable(long carnum, long long datetime, std::string columnName, double storedata, endpoint <CarData*, CarData* > inputqadd) {
int DatabaseConnector::addDataToTable(CarData *receivedData) {
int pass = 0;
long carIDNum, date, time;
long data;//change to std::string when overload implemented
std::map<std::string, std::string> ::iterator iter;
		  //create look to check that the KeyNames match the column order using for loop comparing it to the known string array 
									//does not need to be order dependent. For best perf only do this if the error 1054 is returned
	std::string str1 = "INSERT INTO car";
	std::string str2 = "";
	str2 = std::to_string(receivedData->get(ID,&carIDNum));
	std::string str3 = " (";
	iter = keyMap.begin();
	iter++;
	for (iter; iter != keyMap.end(); iter++) {
		str3.append(",");
		str3.append(iter->second);//Paul is making map to get column names*****
	}
	std::string str7 = ") VALUES (";
	iter = keyMap.begin();
	iter++;
	for (iter; iter != keyMap.end(); iter++) {
		str7.append(",");
		str7.append(std::to_string(receivedData->get(iter->second,&data)));//get all of the key values***** will also need map to have dataTypes
	}
	std::string str8 = ");";
	std::string NewCarTable = str1 + str2 + str3  + str7 + str8;
	const char* cstr = new char[NewCarTable.length() + 1];
	cstr = new char[NewCarTable.length() + 1];
	cstr = NewCarTable.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {
		return 0;
	}
	else
		return mysql_errno(&mysql);//Failed
}


//int DatabaseConnector::getDataTimestamp(long carnum, long long minValue, long long maxValue, endpoint <CarData*, CarData* > outputq) {//get data for all columns. if timerange not specified then give everything. Want to be able to refine by timestamp
int DatabaseConnector::getDataTimestamp(long carnum, long long minValue, long long maxValue) {
//SELECT * FROM car# WHERE timestamp > # AND timestamp < #;  
	//Can add sorting of results with "ORDER BY timestamp;"
	int pass = 0;
	int numRow = 0;
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
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {
		result = mysql_store_result(&mysql);
		str1 = "car";
		str2 = std::to_string(carnum);
		std::string TableName = str1 + str2;
		const char* cstr1 = new char[TableName.length() + 1];
		cstr1 = TableName.c_str();
		MYSQL_RES *tbl_cols = mysql_list_fields(&mysql, cstr1, "f%");

		unsigned int field_cnt = mysql_num_fields(tbl_cols);
		printf("Number of columns: %d\n", field_cnt);
		while (row = mysql_fetch_row(result)) {//put into cardata object
			for (unsigned int i = 0; i < field_cnt; i++) {
				//outputq.send(row[i]);
				carRowData[numRow][i] = *row[i];
				printf("%s\t", row[i]);
			}
			numRow++;
			printf("\n");

		}
		mysql_free_result(tbl_cols);
		mysql_free_result(result);
		return 0;
	}
	else
		return mysql_errno(&mysql);//Failed
}
//Create database Create a database on the sql server.		create database [databasename];
int DatabaseConnector::createDatabase() {
	int pass = 0;
	std::string str1 = "CREATE DATABASE ";
	std::string str2 = database;
	std::string finalString = str1 + str2;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {;
		return 0;
	}
	else {
		return mysql_errno(&mysql);//Failed
	}
}

//Get Column names and associated types from database run on init and make function to append to this list when a new column is added
//column names will be unique key 
int DatabaseConnector::getColumnTypes(long carnum) {
	int pass = 0;
	int rowCnt = 0;
	std::string str1 = "SHOW COLUMNS FROM car";
	std::string str2 = std::to_string(carnum);
	std::string finalString = str1 + str2;
	const char* cstr = new char[finalString.length() + 1];
	cstr = finalString.c_str();
	pass = mysql_query(&mysql, cstr);
	if (pass == 0) {
		result = mysql_store_result(&mysql);
		while ((row = mysql_fetch_row(result))) {
			for (int i = 0; i < 2; i++) {
				if(columnDataTypes[rowCnt] != row[i]){
					columnDataTypes[endpointOfColumnTypeList] = row[i];
					endpointOfColumnTypeList++;
				}
				rowCnt++;
			}
		}
		mysql_free_result(result);
		return 0;
	}
	else {
		return mysql_errno(&mysql);//Failed
	}
}

//update table UPDATE [table name] SET Select_priv = 'Y',Insert_priv = 'Y',Update_priv = 'Y' where [field name] = 'user';
//int DatabaseConnector::tableUpdate(long carnum, int uniqueID,std::string columnName, double updatedValue, endpoint <CarData*, CarData* > inputqupdate) {//can also use timestamp instead of uniqueID. Also can add multiple column update.
int DatabaseConnector::tableUpdate(long carnum, int uniqueID, std::string columnName, double updatedValue) {
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
		return 0;
	}
	else {
		return mysql_errno(&mysql);//Failed
	}
}

//Close connection		 mysql_close(&mysql)
int DatabaseConnector::closeConnection() {
	mysql_close(&mysql);
	return 0;
}

//free result

int DatabaseConnector::freeResult() {
	mysql_free_result(result);
	return 0;
}

int DatabaseConnector::selectDatabase() {
	int pass = 0;
	pass = mysql_select_db(&mysql, database);
	if (pass == 0) {
		return 0;
	}
	else {
		return mysql_errno(&mysql);//Failed
	}
}

//Add Sanatize Function that cleans inputq and breaks it up into something usable for storing. AKA strings


////////////////////////////////////////////////////////Public Functions///////////////////////////////////////////////////////////////////////////////////////

int DatabaseConnector::InitializeDatabase() {
	initDB(nullptr);
	endpointOfColumnTypeList = 0;
	int ErrorNum = createDatabase();

	if (ErrorNum != 1007 && ErrorNum != 0) {
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;
	}

	int ErrorNum2 = selectDatabase();

	if (ErrorNum2 != 0) {
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;
	}

	int ErrorNum3 = getColumnTypes(1);

	if (ErrorNum3 != 0 || ErrorNum3 != 1146) {
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;
	}

	for (int i = 0; i < 128; i++) {
		knownCarTables[i] = 0;
	}
	return 0;
}

int DatabaseConnector::AddData(CarData *receivedData) {
	int ErrorNum = 0;
	long carnum;
	if (knownCarTables[receivedData->get(ID,&carnum)] == 0) {
		knownCarTables[receivedData->get(ID, &carnum)] = 1;
		ErrorNum = createTable(receivedData);
	}
	if (ErrorNum != 0) {
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;
	}
	
	int ErrorNum2 = addDataToTable(receivedData);
	 if (ErrorNum2 != 0) {
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;
	}
	return 0;
}

int DatabaseConnector::GetData(long carnum, long long minValue, long long maxValue) {
	int ErrorNum = getDataTimestamp(carnum, minValue, maxValue);
	if (ErrorNum != 0) {
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;
	}
	CarData* receivedBoxData;
	while (isRunning) {
		while (boxDataQ->receiveQsize() > 0) {
			boxDataQ->receive(&receivedBoxData);
			outputCache->feed(receivedBoxData);
			/////////////////<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		}
	}

	return 0;
}

int DatabaseConnector::UpdateData(long carnum, int uniqueID, std::string columnName, double updatedValue) {
	
	int ErrorNum = tableUpdate(carnum, uniqueID, columnName, updatedValue);
	if (ErrorNum != 0) {
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;
	}

	return 0;
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
		int ErrorNum = mysql_errno(&mysql);
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;//Failed
	}
}

int DatabaseConnector::dropTable(long carnum) {
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
		return 0;
	}
	else {
		int ErrorNum = mysql_errno(&mysql);
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;//Failed
	}
}

//delete row of data from table   Delete a row(s) from a table.		DELETE from [table name] where [field name] = 'whatever';
int DatabaseConnector::dropRowFromTable(long carnum, long long timestamp) {
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
		return 0;
	}
	else {
		int ErrorNum = mysql_errno(&mysql);
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;//Failed
	}
}

//delete column from table  Delete a column.	alter table [table name] drop column [column name];
int DatabaseConnector::dropColumn(long carnum, std::string columnName) {
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
		return 0;
	}
	else {
		int ErrorNum = mysql_errno(&mysql);
		wxLogError(_(std::to_string(ErrorNum) + mysql_error(&mysql)));
		return 1;//Failed
	}
}

void DatabaseConnector::runDatabaseThread()
{
	CarData* receivedData;
	CarData* receivedBoxData;
	InitializeDatabase();
	while (isRunning) {
		while (dataQ->receiveQsize() > 0) {
			dataQ->receive(&receivedData);
			//AddData(receivedData);
			wxLogDebug("Database connector received a cardata object");
			outputCache->feed(receivedData);
			wxLogDebug("Database connector sent a cardata object");
		}

		while (boxDataQ->receiveQsize() > 0) {
			boxDataQ->receive(&receivedBoxData);
			outputCache->feed(receivedBoxData);
		}
	}
}

