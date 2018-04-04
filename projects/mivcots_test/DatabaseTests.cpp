#include "pch.h"
#include "DatabaseConnector/DatabaseConnector.h"
#include "DatabaseConnector\DatabaseConnector.cpp"


//Returns 0 for success or 1 for fail
TEST(DatabaseTests, Shutdown) {
	int carnum = 1;
	long long datetime = 2018033004040000;//Year[4]Month[2]Day[2]24Hour[2]Minutes[2]Seconds[2]MiliSeconds[2]			2018-03-30-04-04-00-00
	std::string sensortype = "PowerLevel";
	double data = 500;
	std::string sensorvar = "double";
	long long minValue = 2018033004040000;
	long long maxValue = 2018033004041000;
	std::string database = "mivcots";

	DatabaseConnector test;
	test.initDB();
	test.createDatabase(database);

	test.shutdown();

	SUCCEED();
}