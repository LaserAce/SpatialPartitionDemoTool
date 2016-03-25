#ifndef _PARTITION_H_
#define _PARTITION_H_

#include "gameobject.h"
#include <list>
using std::list;

class PartitionObject;
class VBShape;
struct FindPointTest;
struct CheckPointTest;

class Partition : public GameObject
{
public:
	void AssignObject(PartitionObject* _object);
	virtual void Insert(PartitionObject* _object) = 0;
	virtual void Remove(PartitionObject* _object) = 0;
	virtual void Update(PartitionObject* _object) = 0;
	virtual void Clear() = 0;
	virtual void Tick(GameData* _GD) = 0;
	virtual void Draw(DrawData* _DD) = 0;
	virtual Partition* FindPartition(Vector3 _pos, int _level) = 0;

	virtual void Rebuild(list<PartitionObject*> _objects);
	virtual void Rebuild() = 0;

	virtual void FindTest(FindPointTest* _test)= 0;
	virtual list<PartitionObject*> CheckTest(CheckPointTest* _test) = 0;
	void Reset();

	VBShape* GetOutline(){ return m_outline; }
	virtual void HighlightObjects(Color _col);
	virtual void UnHighlightObjects();

	bool ShapeQuery(Vector2 _upperLeft, Vector2 _lowerRight);

	int* GetMaxObjects() { return &m_maxObjects; }
	int* GetMaxLevels() { return &m_maxLevels; }

protected:
	list<PartitionObject*> m_objects;   //The objects that a partition holds
	int m_defaultMaxObjects = 10;
	int m_defaultMaxLevels = 5;
	int m_maxObjects = 10;   //Minimum number of objects a partition can hold before it tries to split
	int m_maxLevels = 5;   //Maximum level of splitting that can happen
	int m_level = 0;   //The starting level
	VBShape* m_outline;

	bool PointQuery(Vector3 _pos, Vector2 _upperLeft, Vector2 _lowerRight);
	
	Vector2 m_upperLeft;
	Vector2 m_lowerRight;
private:
	
	
	
};

#endif