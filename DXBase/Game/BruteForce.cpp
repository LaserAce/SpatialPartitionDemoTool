#include "BruteForce.h"
#include "PartitionManager.h"
#include "PartitionObject.h"
#include "VBShape.h"
#include "StatisticTest.h"

BruteForce::BruteForce()
{
	m_defaultMaxObjects = 0;
	m_defaultMaxLevels = 0;
	
	m_maxLevels = m_defaultMaxLevels;
	m_maxObjects = m_defaultMaxObjects;
	
	m_parent = nullptr;
	m_outline = nullptr;
}

BruteForce::~BruteForce()
{
	Clear();
}

void BruteForce::Insert(PartitionObject* _object)
{
	AssignObject(_object);
}

void BruteForce::Remove(PartitionObject* _object)
{
	m_objects.erase(_object->GetPartitionIterator());
	_object->SetCurrentPartition(nullptr);
}

void BruteForce::Update(PartitionObject* _object)
{
	Remove(_object);
	_object->s_partitionManager->GetCurrentRoot()->Insert(_object);
}

list<PartitionObject*> BruteForce::Retrieve(PartitionObject* _object)
{
	_object;
	list<PartitionObject*> retrieve;
	return retrieve;
}

//Clears away all objects and delete all children nodes
void BruteForce::Clear()
{
	//Set the current partition each object is part of to nullptr then clear the objects
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->SetCurrentPartition(nullptr);
	}
	m_objects.clear();
}

Partition* BruteForce::FindPartition(Vector3 _pos, int _level)
{
	_pos;
	_level;
	return nullptr;
}

void BruteForce::Rebuild(list<PartitionObject*> _objects)
{
	Clear();
	for (list<PartitionObject*>::iterator it = _objects.begin(); it != _objects.end(); ++it)
	{
		AssignObject(*it);
	}
}

void BruteForce::Rebuild()
{

}

void BruteForce::Test(StatisticTest* _test)
{
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (PointQuery((*it)->GetGameObject()->GetPos(), _test->upperLeft, _test->lowerRight))
		{
			++_test->pointsFound;
		}
		++_test->numberChecks;
	}
}

void BruteForce::Tick(GameData* _GD)
{
	_GD;
}

void BruteForce::Draw(DrawData* _DD)
{
	_DD;
}