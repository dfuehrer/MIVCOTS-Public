#include "pch.h"
#include "DatabaseConnector/DatabaseConnector.h"
#include "DatabaseConnector\DatabaseConnector.cpp"

//Returns 0 for success or 1 for fail

TEST(DatabaseTests, DropColumnRowAndTableTest) {
	int carnum = 1;
	long long datetime = 2018033004040000;//Year[4]Month[2]Day[2]24Hour[2]Minutes[2]Seconds[2]MiliSeconds[2]			2018-03-30-04-04-00-00
	std::string sensortype = "PowerLevel";
	double data = 500;
	std::string sensorvar = "double";
	long long minValue = 2018033004040000;
	long long maxValue = 2018033004041000;
	std::string database = "mivcots";
	int addDataIterations = 3;
	double updatedata = 6969;
	int updateUniqueID = 5;
	long long dropDateTime = 2018033004040050;
	DatabaseConnector test;
	//CarData* car;

	ASSERT_EQ(test.InitializeDatabase(database), 0);
	for (int i = 0; i < addDataIterations; i++) {
		EXPECT_EQ(test.AddData(carnum, sensortype, sensorvar, datetime, data), 0);
		datetime = datetime + 10;
		data = data + 250;
	}
	ASSERT_EQ(test.dropRowFromTable(carnum, dropDateTime), 0);
	ASSERT_EQ(test.dropColumn(carnum, sensortype), 0);
	ASSERT_EQ(test.dropTable(carnum), 0);
}

TEST(DatabaseTests, AddDataTest) {
	int carnum = 1;
	long long datetime = 2018033004040000;//Year[4]Month[2]Day[2]24Hour[2]Minutes[2]Seconds[2]MiliSeconds[2]			2018-03-30-04-04-00-00
	std::string sensortype = "PowerLevel";
	double data = 500;
	std::string sensorvar = "double";
	long long minValue = 2018033004040000;
	long long maxValue = 2018033004041000;
	std::string database = "mivcots";
	int addDataIterations = 3;
	double updatedata = 6969;
	int updateUniqueID = 5;
	DatabaseConnector test;

	//Everything should return 0 for PASS and 1 for FAIL

	//ASSERT_EQ(val1,val2); Fatal Assertion
	//EXPECT_EQ(val1,val2); NonFatal Assertion
	ASSERT_EQ(test.InitializeDatabase(database), 0);
	for (int i = 0; i < addDataIterations; i++) {
		EXPECT_EQ(test.AddData(carnum, sensortype, sensorvar, datetime, data), 0);
		datetime = datetime + 10;
		data = data + 250;
	}
	ASSERT_EQ(test.UpdateData(carnum, updateUniqueID, sensortype, updatedata), 0);
}

