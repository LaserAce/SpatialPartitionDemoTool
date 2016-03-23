#ifndef _STATISTIC_TEST_H_
#define _STATISTIC_TEST_H_

#include <string>

using std::string;

enum TestType
{
	TEST_ALL_POINTS,
	TEST_QUERY_ZONE,
	ALL_TESTS
};

class StatisticTest
{
public:
	StatisticTest() { ++numTests; }

	static int numTests;
	int internalID;
	string partitionName;
	string testType;
	float timeTaken;
	int numberChecks;
	int pointsFound;
	int nodesTravelled;
};
#endif
