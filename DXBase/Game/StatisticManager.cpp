#include "StatisticManager.h"
#include "StatisticTest.h"
#include "PartitionManager.h"

using std::to_string;

StatisticManager* StatisticManager::singleton = nullptr;

StatisticManager::StatisticManager()
{
	singleton = this;
}

StatisticManager::~StatisticManager()
{
	TwDeleteBar(rightUI);
	for (list<StatisticTest*>::iterator it = m_tests.begin(); it != m_tests.end(); ++it)
	{
		delete (*it);
	}
	m_tests.clear();
}

void StatisticManager::SetupTwBar()
{
	rightUI = TwNewBar("rightUI");
	TwDefine(" rightUI label='Statistic Tracker' ");
	TwDefine(" rightUI movable=false ");
	TwDefine(" rightUI resizable=false ");
	TwDefine(" rightUI color='122 122 122' ");
	TwDefine(" rightUI iconifiable=false ");

	TwAddButton(rightUI, "removeall", RemoveAllTests, nullptr, " Label='Remove All Tests' ");

	AdjustSize();
}

void StatisticManager::AdjustSize()
{
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &m_winSize);

	m_size = 0.15;
	int barSize[2] = { (int)((float)m_winSize.right * m_size), m_winSize.bottom };
	TwSetParam(rightUI, NULL, "size", TW_PARAM_INT32, 2, barSize);
	int barPosition[3] = { (int)((float)m_winSize.right - ((float)m_winSize.right * m_size)), 0, 0 };
	TwSetParam(rightUI, NULL, "position", TW_PARAM_INT32, 3, barPosition);
}

void StatisticManager::Tick(GameData* _GD)
{
	GameObject::Tick(_GD);
}

void StatisticManager::Draw(DrawData* _DD)
{
	_DD;
}

void StatisticManager::GenerateTest(Vector2 _upperLeft, Vector2 _lowerRight)
{
	StatisticTest* newTest = new StatisticTest();
	newTest->internalID = numTests;
	newTest->upperLeft = _upperLeft;
	newTest->lowerRight = _lowerRight;
	ULONGLONG timeAtBegin = GetTickCount64();
	
	PartitionManager::Singleton()->Test(newTest);

	newTest->timeTaken = (int)(GetTickCount64() - timeAtBegin);

	string name = "Test" + to_string(numTests);
	string group = " group='Test " + to_string(numTests) + "' label='";

	TwAddButton(rightUI, (name + "method").c_str(), NULL, NULL, (group + "Method: " + PartitionManager::Singleton()->GetCurrentPartitionName() + " '").c_str());
	TwAddButton(rightUI, (name + "time").c_str(), NULL, NULL, (group + ("Time Taken: " + to_string(newTest->timeTaken)) + " '").c_str());
	TwAddButton(rightUI, (name + "points").c_str(), NULL, NULL, (group + ("Points Found: " + to_string(newTest->pointsFound)) + " '").c_str());
	TwAddButton(rightUI, (name + "checks").c_str(), NULL, NULL, (group + ("Number of Checks: " + to_string(newTest->numberChecks)) + " '").c_str());
	TwAddButton(rightUI, (name + "nodes").c_str(), NULL, NULL, (group + ("Nodes Traversed: " + to_string(newTest->nodesTravelled)) + " '").c_str());
	TwAddButton(rightUI, (name + "removetest").c_str(), RemoveTest, newTest, (group + "Remove Test '").c_str());

	++numTests;
	m_tests.push_back(newTest);
	newTest->it_tests = prev(m_tests.end());
}

void TW_CALL StatisticManager::RemoveTest(void* _clientData)
{
	StatisticTest* _test = (StatisticTest*)_clientData;
	TwBar* _bar = TwGetBarByName("rightUI");
	string name = "Test" + to_string(_test->internalID);
	TwRemoveVar(_bar, (name + "method").c_str());
	TwRemoveVar(_bar, (name + "time").c_str());
	TwRemoveVar(_bar, (name + "removetest").c_str());
	TwRemoveVar(_bar, (name + "points").c_str());
	TwRemoveVar(_bar, (name + "checks").c_str());
	TwRemoveVar(_bar, (name + "nodes").c_str());

	//m_tests.erase(it_gameObjects); Remove from list not necessary
	_clientData;
}

void TW_CALL StatisticManager::RemoveAllTests(void* _clientData)
{
	TwBar* _bar = TwGetBarByName("rightUI");
	TwRemoveAllVars(_bar);
	TwAddButton(_bar, "removeall", RemoveAllTests, nullptr, " Label='Remove All Tests' ");
	//m_tests.erase(it_gameObjects); Remove all componenets from list
	_clientData;
}

