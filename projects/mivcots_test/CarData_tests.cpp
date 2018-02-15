#include "pch.h"
#include "CarData/CarData.h"
#include "CarData/CarData.cpp"

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