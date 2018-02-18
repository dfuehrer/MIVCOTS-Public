#include "pch.h"
#include "CarData/CarData.h"
#include "CarData/CarData.cpp"
#include "InterThreadComm/InterThreadComm.h"
#include "InterThreadComm/InterThreadComm.cpp"

TEST(CarDataTests, basic) {
	CarData uut;
	
	char testKey[3] = "A";
	uut.addKey(testKey);

	long input = -3;
	uut.set(testKey, input);

	long retreive;
	uut.get(testKey, &retreive);

	ASSERT_EQ(retreive, -3);
}

#include <string>

TEST(InterThreadCommTests, basic) {
	InterThreadComm<std::string, int> uut;
	endpoint<std::string, int>* endpoint1 = uut.getEndpoint1();
	endpoint<int, std::string>* endpoint2 = uut.getEndpoint2();

	endpoint1->send("This is a test");
	endpoint1->send("Part 2");
	endpoint2->send(42);
	endpoint2->send(25);

	std::string dest1;
	std::string dest2;
	int dest3;
	int dest4;

	int rc1 = endpoint2->receive(&dest1);
	int rc2 = endpoint2->receive(&dest2);
	int rc3 = endpoint1->receive(&dest3);
	int rc4 = endpoint1->receive(&dest4);

	ASSERT_EQ(rc1 + rc2 + rc3 + rc4, 0);
	EXPECT_EQ(dest1, "This is a test");
	EXPECT_EQ(dest2, "Part 2");
	EXPECT_EQ(dest3, 42);
	EXPECT_EQ(dest4, 25);
}