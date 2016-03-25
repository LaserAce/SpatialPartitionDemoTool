#ifndef _PARTITION_MANAGER_H_
#define _PARTITION_MANAGER_H_

#include "gameobject.h"
#include "PartitionMethods.h"

#include <vector>
#include <list>
#include <string>
using std::vector;
using std::list;
using std::string;

class Partition;
struct FindPointTest;
struct CheckPointTest;

class PartitionManager : public GameObject
{
public:
	PartitionManager();
	~PartitionManager();

	static PartitionManager* Singleton() { return singleton; }

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	void RebuildPartition();
	void DeletePoints();

	Partition* GetCurrentRoot();
	list<PartitionObject*>::iterator AddToManagerList(PartitionObject* _object);
	void RemoveFromManagerList(list<PartitionObject*>::iterator _iterator);

	void SetActiveMethod(PartitionMethods _method);
	PartitionMethods* GetActiveMethod();
	void SetViewLevel(int _i);
	int* GetViewLevel();
	bool* GetDebugVisible(){ return &m_debugVisible; }
	bool* GetHighlight() { return &m_highlight; }
	void UnHighlightPartition();
	void HighlightPartition();
	FindPointTest* FindTest(FindPointTest* _test);
	CheckPointTest* CheckTest(CheckPointTest* _test);
	string GetCurrentPartitionName();
	void ResetPartition();

private:
	static PartitionManager* singleton;

	vector<Partition*> m_partitionMethods;
	PartitionMethods m_activeMethod;
	int m_viewLevel;
	bool m_debugVisible;
	bool m_highlight;
	Partition* p_highlightedPartition;
	

	list<PartitionObject*> p_allPartitionObjects;

};

#endif