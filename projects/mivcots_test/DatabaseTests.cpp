#include "pch.h"
#include "DatabaseConnector/DatabaseConnector.h"
#include "DatabaseConnector\DatabaseConnector.cpp"

//Returns 0 for success or 1 for fail
TEST(DatabaseTests, Overall) {
	int carnum = 1;
	long long datetime = 2018033004040000;//Year[4]Month[2]Day[2]24Hour[2]Minutes[2]Seconds[2]MiliSeconds[2]			2018-03-30-04-04-00-00
	std::string sensortype = "PowerLevel";
	double data = 500;
	std::string sensorvar = "double";
	long long minValue = 2018033004040000;
	long long maxValue = 2018033004041000;
	std::string database = "mivcots";
	int addDataIterations = 50;
	double updatedata = 6969;
	int updateUniqueID = 5;
	DatabaseConnector test;

	//Everything should return 0 for PASS and 1 for FAIL

	//ASSERT_EQ(val1,val2); Fatal Assertion
	//EXPECT_EQ(val1,val2); NonFatal Assertion
	ASSERT_EQ(test.initDB(nullptr), 0);
	ASSERT_EQ(test.createDatabase(database), 0);
	ASSERT_EQ(test.createTable(carnum), 0);
	ASSERT_EQ(test.addNewColumn(carnum, sensortype, sensorvar), 0);
	for (int i = 0; i < addDataIterations; i++) {
		ASSERT_EQ(test.addDataToTable(carnum, datetime, sensortype, data), 0);
		datetime = datetime + 10;
		data = data + 250;
	}
	ASSERT_EQ(test.tableUpdate(carnum, updateUniqueID, sensortype, updatedata), 0);

	test.shutdown();

}