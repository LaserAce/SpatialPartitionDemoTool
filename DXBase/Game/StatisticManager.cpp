#include "StatisticManager.h"
#include "StatisticTest.h"

StatisticManager* StatisticManager::singleton = nullptr;
int StatisticTest::numTests = 0;

StatisticManager::StatisticManager()
{
	singleton = this;
}

StatisticManager::~StatisticManager()
{

}

void StatisticManager::Tick(GameData* _GD)
{
	GameObject::Tick(_GD);
}

void StatisticManager::Draw(DrawData* _DD)
{
	
}

void StatisticManager::GenerateTest(TestType _test)
{
	StatisticTest* newTest = new StatisticTest();
	newTest->internalID = StatisticTest::numTests;
	ULONGLONG timeAtBegin = GetTickCount64();
	switch (_test)
	{
	case TEST_ALL_POINTS:
		AllPointsTest(newTest);
		break;
	case TEST_QUERY_ZONE:
		QueryZoneTest(newTest);
		break;
	}
	newTest->timeTaken = (int)(GetTickCount64() - timeAtBegin);
}

void StatisticManager::AllPointsTest(StatisticTest* _test)
{

}


void StatisticManager::QueryZoneTest(StatisticTest* _test)
{

}

