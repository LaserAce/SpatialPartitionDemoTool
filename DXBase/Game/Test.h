#ifndef _TEST_H_
#define _TEST_H_

#include "SimpleMath.h"
#include <string>
#include <list>

using namespace DirectX;
using namespace SimpleMath;
using std::string;
using std::list;

struct Test
{
public:
	list<Test*>::iterator it_tests;

	int internalID;
	string partitionName;
	string testType;
	int timeTaken;
};

struct FindPointTest : public Test
{
	Vector2 upperLeft;
	Vector2 lowerRight;

	int numberPointChecks;
	int numberNodeChecks;
	int pointsFound;
	int nodesTravelled;
};

struct CheckPointTest : public Test
{
	Vector2 upperLeft;
	Vector2 lowerRight;

	int numberPointToPointTests;
};

#endif
