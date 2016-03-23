#ifndef _PARTITION_H_
#define _PARTITION_H_

#include "gameobject.h"
#include <list>
using std::list;

class PartitionObject;
class VBShape;
class Partition : public GameObject
{
public:
	void AssignObject(PartitionObject* _object);
	virtual void Insert(PartitionObject* _object) = 0;
	virtual void Remove(PartitionObject* _object) = 0;
	virtual void Update(PartitionObject* _object) = 0;
	virtual list<PartitionObject*> Retrieve(PartitionObject* _object) = 0;
	virtual void Clear() = 0;
	virtual void Tick(GameData* _GD) = 0;
	virtual void Draw(DrawData* _DD) = 0;
	virtual Partition* FindPartition(Vector3 _pos, int _level) = 0;

	virtual void Rebuild(list<PartitionObject*> _objects) = 0;
	virtual void Rebuild() = 0;

	VBShape* GetOutline(){ return m_outline; }
	virtual void HighlightObjects(Color _col);
	virtual void UnHighlightObjects();

protected:
	list<PartitionObject*> m_objects;   //The objects that a partition holds
	int m_maxObjects = 10;   //Minimum number of objects a partition can hold before it tries to split
	int m_maxLevels = 5;   //Maximum level of splitting that can happen
	int m_level = 0;   //The starting level
	VBShape* m_outline;
private:
	
	
	
};

#endif