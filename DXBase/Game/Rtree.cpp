//Placeholder for incomplete R-Tree
#include "Rtree.h"
#include "PartitionManager.h"
#include "PartitionObject.h"
#include "VBShape.h"
#include "Test.h"

Rtree::Rtree()
{
	m_defaultMaxObjects = 0;
	m_defaultMaxLevels = 0;

	m_maxLevels = m_defaultMaxLevels;
	m_maxObjects = m_defaultMaxObjects;

	m_parent = nullptr;
	m_outline = nullptr;
}

Rtree::~Rtree()
{
	Clear();
}

void Rtree::Insert(PartitionObject* _object)
{
	AssignObject(_object);
}

void Rtree::Remove(PartitionObject* _object)
{
	m_objects.erase(_object->GetPartitionIterator());
	_object->SetCurrentPartition(nullptr);
}

void Rtree::Update(PartitionObject* _object)
{
	Remove(_object);
	PartitionManager::Singleton()->GetCurrentRoot()->Insert(_object);
}

//Clears away all objects and delete all children nodes
void Rtree::Clear()
{
	//Set the current partition each object is part of to nullptr then clear the objects
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->SetCurrentPartition(nullptr);
	}
	m_objects.clear();
}

Partition* Rtree::FindPartition(Vector3 _pos, int _level)
{
	_pos;
	_level;
	return nullptr;
}

void Rtree::Rebuild()
{

}

void Rtree::FindTest(FindPointTest* _test)
{
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (PointQuery((*it)->GetGameObject()->GetPos(), _test->upperLeft, _test->lowerRight))
		{
			++_test->pointsFound;
		}
		++_test->numberPointChecks;
	}
}

list<PartitionObject*> Rtree::CheckTest(CheckPointTest* _test)
{
	list<PartitionObject*> _checkObjects;
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (PointQuery((*it)->GetGameObject()->GetPos(), _test->upperLeft, _test->lowerRight))
		{
			_checkObjects.push_back(*it);
		}
	}
	return _checkObjects;
}

void Rtree::Tick(GameData* _GD)
{
	_GD;
}

void Rtree::Draw(DrawData* _DD)
{
	_DD;
}