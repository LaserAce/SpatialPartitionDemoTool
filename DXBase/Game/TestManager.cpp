#include "TestManager.h"
#include "Test.h"
#include "PartitionManager.h"

using std::to_string;

TestManager* TestManager::singleton = nullptr;

TestManager::TestManager()
{
	singleton = this;
}

TestManager::~TestManager()
{
	TwDeleteBar(rightUI);
	for (list<Test*>::iterator it = m_tests.begin(); it != m_tests.end(); ++it)
	{
		delete (*it);
	}
	m_tests.clear();
}

void TestManager::Tick(GameData* _GD)
{
	GameObject::Tick(_GD);
}

void TestManager::Draw(DrawData* _DD)
{
	_DD;
}

void TestManager::SetupTwBar()
{
	//Setup the right bar
	rightUI = TwNewBar("rightUI");
	TwDefine(" rightUI label='Test Tracker' ");
	TwDefine(" rightUI movable=false ");
	TwDefine(" rightUI resizable=false ");
	TwDefine(" rightUI color='122 122 122' ");
	TwDefine(" rightUI iconifiable=false ");

	TwAddButton(rightUI, "removeall", RemoveAllTests, nullptr, " Label='Remove All Tests' ");

	AdjustSize();
}

void TestManager::AdjustSize()
{
	//Adjusts the size based on screen dimensions
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &m_winSize);

	m_size = 0.15;
	int barSize[2] = { (int)((float)m_winSize.right * m_size), m_winSize.bottom };
	TwSetParam(rightUI, NULL, "size", TW_PARAM_INT32, 2, barSize);
	int barPosition[3] = { (int)((float)m_winSize.right - ((float)m_winSize.right * m_size)), 0, 0 };
	TwSetParam(rightUI, NULL, "position", TW_PARAM_INT32, 3, barPosition);
}

void TestManager::GenerateFindTest(Vector2 _upperLeft, Vector2 _lowerRight)
{
	//Initilise test values
	FindPointTest* newTest = new FindPointTest();
	newTest->internalID = numTests;
	newTest->testType = "Find Point Test";
	newTest->upperLeft = _upperLeft;
	newTest->lowerRight = _lowerRight;

	//Time and perform the find all points within zone test
	ULONGLONG timeAtBegin = GetTickCount64();
	PartitionManager::Singleton()->FindTest(newTest);
	newTest->timeTaken = (int)(GetTickCount64() - timeAtBegin);

	//Generate an antweakbar display using test
	string name = "Test" + to_string(numTests);
	string group = " group='Test " + to_string(numTests) + "' label='";
	TwAddButton(rightUI, (name + "type").c_str(), NULL, NULL, (group + "Test Type: " + newTest->testType + " '").c_str());
	TwAddButton(rightUI, (name + "method").c_str(), NULL, NULL, (group + "Method: " + PartitionManager::Singleton()->GetCurrentPartitionName() + " '").c_str());
	TwAddButton(rightUI, (name + "time").c_str(), NULL, NULL, (group + ("Time Taken: " + to_string(newTest->timeTaken)) + " '").c_str());
	TwAddButton(rightUI, (name + "points").c_str(), NULL, NULL, (group + ("Points Found: " + to_string(newTest->pointsFound)) + " '").c_str());
	TwAddButton(rightUI, (name + "checks").c_str(), NULL, NULL, (group + ("Number of Checks: " + to_string(newTest->numberPointChecks)) + " '").c_str());
	TwAddButton(rightUI, (name + "nodechecks").c_str(), NULL, NULL, (group + ("Number of Node Checks: " + to_string(newTest->numberNodeChecks)) + " '").c_str());
	TwAddButton(rightUI, (name + "nodes").c_str(), NULL, NULL, (group + ("Nodes Traversed: " + to_string(newTest->nodesTravelled)) + " '").c_str());
	TwAddButton(rightUI, (name + "removetest").c_str(), RemoveTest, newTest, (group + "Remove Test '").c_str());

	//Increment test and add to list for further potential use
	++numTests;
	m_tests.push_back(newTest);
	newTest->it_tests = prev(m_tests.end());
}

void TestManager::GenerateCheckTest(Vector2 _upperLeft, Vector2 _lowerRight)
{
	//Initilise test values
	CheckPointTest* newTest = new CheckPointTest();
	newTest->internalID = numTests;
	newTest->testType = "Check Point Test";
	newTest->upperLeft = _upperLeft;
	newTest->lowerRight = _lowerRight;

	//Perform check test by testing each found object with each other
	PartitionManager::Singleton()->CheckTest(newTest);

	//Generate an antweakbar display using test
	string name = "Test" + to_string(numTests);
	string group = " group='Test " + to_string(numTests) + "' label='";
	TwAddButton(rightUI, (name + "type").c_str(), NULL, NULL, (group + "Test Type: " + newTest->testType + " '").c_str());
	TwAddButton(rightUI, (name + "method").c_str(), NULL, NULL, (group + "Method: " + PartitionManager::Singleton()->GetCurrentPartitionName() + " '").c_str());
	TwAddButton(rightUI, (name + "time").c_str(), NULL, NULL, (group + ("Time Taken: " + to_string(newTest->timeTaken)) + " '").c_str());
	TwAddButton(rightUI, (name + "checkchecks").c_str(), NULL, NULL, (group + ("Point to Point Checks: " + to_string(newTest->numberPointToPointTests)) + " '").c_str());
	TwAddButton(rightUI, (name + "removetest").c_str(), RemoveTest, newTest, (group + "Remove Test '").c_str());

	//Increment test and add to list for further potential use
	++numTests;
	m_tests.push_back(newTest);
	newTest->it_tests = prev(m_tests.end());
}

Test* TestManager::BeginBuildTest()
{
	//Initilise test values
	Test* newTest = new Test();
	newTest->internalID = numTests;
	newTest->testType = "Build Test";
	newTest->timeTaken = (int)GetTickCount64();
	
	//Increment test and add to list for further potential use
	++numTests;
	m_tests.push_back(newTest);
	newTest->it_tests = prev(m_tests.end());

	//Return to partition manager so it can pass back to end test
	return newTest;
}

void TestManager::EndBuildTest(Test* _test)
{
	//Update the time taken
	_test->timeTaken = (int)(GetTickCount64() - _test->timeTaken);

	//Generate an antweakbar display using test
	string name = "Test" + to_string(numTests);
	string group = " group='Test " + to_string(numTests) + "' label='";
	TwAddButton(rightUI, (name + "type").c_str(), NULL, NULL, (group + "Test Type: " + _test->testType + " '").c_str());
	TwAddButton(rightUI, (name + "method").c_str(), NULL, NULL, (group + "Method: " + PartitionManager::Singleton()->GetCurrentPartitionName() + " '").c_str());
	TwAddButton(rightUI, (name + "time").c_str(), NULL, NULL, (group + ("Time Taken: " + to_string(_test->timeTaken)) + " '").c_str());
	TwAddButton(rightUI, (name + "removetest").c_str(), RemoveTest, _test, (group + "Remove Test '").c_str());
}

void TW_CALL TestManager::RemoveTest(void* _clientData)
{
	//Removes a single anttweakbar test display
	Test* _test = (Test*)_clientData;
	TwBar* _bar = TwGetBarByName("rightUI");
	string name = "Test" + to_string(_test->internalID);
	TwRemoveVar(_bar, (name + "type").c_str());
	TwRemoveVar(_bar, (name + "method").c_str());
	TwRemoveVar(_bar, (name + "time").c_str());
	TwRemoveVar(_bar, (name + "removetest").c_str());

	if (_test->testType == "Find Point Test")
	{
		TwRemoveVar(_bar, (name + "points").c_str());
		TwRemoveVar(_bar, (name + "checks").c_str());
		TwRemoveVar(_bar, (name + "nodechecks").c_str());
		TwRemoveVar(_bar, (name + "nodes").c_str());
	}
	else if (_test->testType == "Check Point Test")
	{
		TwRemoveVar(_bar, (name + "checkchecks").c_str());
	}

	//m_tests.erase(it_gameObjects); Remove from list not necessary
	_clientData;
}

void TW_CALL TestManager::RemoveAllTests(void* _clientData)
{
	//Removes all anttweakbar tests so far
	TwBar* _bar = TwGetBarByName("rightUI");
	TwRemoveAllVars(_bar);
	TwAddButton(_bar, "removeall", RemoveAllTests, nullptr, " Label='Remove All Tests' ");
	//m_tests.erase(it_gameObjects); Remove all componenets from list
	_clientData;
}

