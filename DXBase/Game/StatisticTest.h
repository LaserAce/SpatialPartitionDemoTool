#ifndef _STATISTIC_TEST_H_
#define _STATISTIC_TEST_H_

#include "SimpleMath.h"
#include <string>
#include <list>

using namespace DirectX;
using namespace SimpleMath;
using std::string;
using std::list;

struct StatisticTest
{
public:
	list<StatisticTest*>::iterator it_tests;

	Vector2 upperLeft;
	Vector2 lowerRight;

	int internalID;
	string partitionName;
	int timeTaken;
	int numberChecks;
	int pointsFound;
	int nodesTravelled;
};
#endif
