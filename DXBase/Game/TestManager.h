#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_

#include "gameobject.h"
#include <list>
#include <AntTweakBar.h>

using std::list;

enum TestType;
struct Test;

class TestManager : public GameObject
{
public:
	TestManager();
	~TestManager();

	static TestManager* Singleton() { return singleton; }

	void SetupTwBar();
	void AdjustSize();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	static void TW_CALL RemoveTest(void* _clientData);
	static void TW_CALL RemoveAllTests(void* _clientData);

	void GenerateFindTest(Vector2 _upperLeft, Vector2 _lowerRight);
	void GenerateCheckTest(Vector2 _upperLeft, Vector2 _lowerRight);
	Test* BeginBuildTest();
	void EndBuildTest(Test* _test);

	bool* GetBuildTest() { return &generateBuildTests; }
protected:
	static TestManager* singleton;

	list<Test*> m_tests;

	int numTests = 0;

	bool generateBuildTests = false;

	RECT m_winSize;
	TwBar* rightUI;
	//lower number takes up more space on screen
	float m_size;

};


#endif