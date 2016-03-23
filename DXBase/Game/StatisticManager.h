#ifndef _STATISTIC_MANAGER_H_
#define _STATISTIC_MANAGER_H_

#include "gameobject.h"
#include <list>

using std::list;

enum TestType;
class StatisticTest;

class StatisticManager : public GameObject
{
public:
	StatisticManager();
	~StatisticManager();

	static StatisticManager* Singleton() { return singleton; }

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	void GenerateTest(TestType _type);
protected:
	static StatisticManager* singleton;

	list<StatisticTest*> m_tests;

	void AllPointsTest(StatisticTest* _test);
	void QueryZoneTest(StatisticTest* _test);

};


#endif