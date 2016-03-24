#ifndef _STATISTIC_MANAGER_H_
#define _STATISTIC_MANAGER_H_

#include "gameobject.h"
#include <list>
#include <AntTweakBar.h>

using std::list;

enum TestType;
struct StatisticTest;

class StatisticManager : public GameObject
{
public:
	StatisticManager();
	~StatisticManager();

	static StatisticManager* Singleton() { return singleton; }

	void SetupTwBar();
	void AdjustSize();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	static void TW_CALL RemoveTest(void* _clientData);
	static void TW_CALL RemoveAllTests(void* _clientData);

	void GenerateTest(Vector2 _upperLeft, Vector2 _lowerRight);
protected:
	static StatisticManager* singleton;

	list<StatisticTest*> m_tests;

	int numTests = 0;

	RECT m_winSize;
	TwBar* rightUI;
	//lower number takes up more space on screen
	float m_size;

};


#endif