#ifndef _PARTITION_MANAGER_H_
#define _PARTITION_MANAGER_H_

#include "gameobject.h"
#include "PartitionMethods.h"

#include <vector>
#include <list>
using std::vector;
using std::list;

class Partition;

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
	bool* GetDebugVisible(){ return &m_debugVisible; };
	void UnHighlightPartition();
	void HighlightPartition();

private:
	static PartitionManager* singleton;

	vector<Partition*> m_partitionMethods;
	PartitionMethods m_activeMethod;
	int m_viewLevel;
	bool m_debugVisible;
	Partition* p_highlightedPartition;
	

	list<PartitionObject*> p_allPartitionObjects;

};

#endif