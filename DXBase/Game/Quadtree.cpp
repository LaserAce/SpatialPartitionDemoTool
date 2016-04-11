#include "Quadtree.h"
#include "PartitionManager.h"
#include "PartitionObject.h"
#include "VBShape.h"
#include "Test.h"

Quadtree::Quadtree(Vector3 _centre, float _halfWidth, int _level, int _maxObjects, int _maxLevels)
{
	m_defaultMaxObjects = 10;
	m_defaultMaxLevels = 5;

	m_maxLevels = _maxLevels;
	m_maxObjects = _maxObjects;

	m_pos = _centre;
	m_halfWidth = _halfWidth;
	m_level = _level;
	m_nodes.resize((int)MAX_CORNERS);

	m_outline = new VBShape();
	m_outline->InitialiseShape("WireCube2D");
	m_outline->SetPos(m_pos);
	m_outline->SetScale(Vector3(_halfWidth,_halfWidth, 1));
	m_outline->Tick(nullptr);

	m_upperLeft = Vector2(_centre.x - _halfWidth, _centre.y + _halfWidth);
	m_lowerRight = Vector2(_centre.x + _halfWidth, _centre.y - _halfWidth);
}

Quadtree::~Quadtree()
{
	Clear();
	delete m_outline;
}

int Quadtree::GetIndex(Vector3 _pos)
{
	Vector3 pos = _pos;
	int index;// = (int)MAX_CORNERS;
	if (pos.y < m_pos.y)
	{
		if (pos.x < m_pos.x)
		{
			index = (int)TOP_LEFT;
		}
		else
		{
			index = (int)TOP_RIGHT;
		}
	}
	else
	{
		if (pos.x < m_pos.x)
		{
			index = (int)BOTTOM_LEFT;
		}
		else
		{
			index = (int)BOTTOM_RIGHT;
		}
	}
	return index;
}

void Quadtree::Insert(PartitionObject* _object)
{
	//If children nodes exist then place straight into them
	if (m_nodes[0])
	{
		int index = GetIndex(_object->GetGameObject()->GetPos());
		m_nodes[index]->Insert(_object);
		return;
	}

	AssignObject(_object);

	//Only try if can go down more levels
	if (m_level < m_maxLevels)
	{
		//Only if the object cap for the quadrant has been reached
		if ((int)m_objects.size() > m_maxObjects)
		{
			if (!m_nodes[0])
			{
				Split();
			}

			//Move all objects down the tree
			for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end();)
			{
				int index = GetIndex((*it)->GetGameObject()->GetPos());
				m_nodes[index]->Insert(*it);
				it = m_objects.erase(it);
			}
		}
	}
}

void Quadtree::Remove(PartitionObject* _object)
{
	m_objects.erase(_object->GetPartitionIterator());
}

void Quadtree::Update(PartitionObject* _object)
{
	Remove(_object);
	PartitionManager::Singleton()->GetCurrentRoot()->Insert(_object);
}

void Quadtree::Split()
{
	//Create nodes for each quadrant
	m_nodes[(int)TOP_RIGHT] = new Quadtree(Vector3(m_pos.x + (m_halfWidth / 2), m_pos.y - (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, m_maxObjects, m_maxLevels);
	m_nodes[(int)TOP_LEFT] = new Quadtree(Vector3(m_pos.x - (m_halfWidth / 2), m_pos.y - (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, m_maxObjects, m_maxLevels);
	m_nodes[(int)BOTTOM_LEFT] = new Quadtree(Vector3(m_pos.x - (m_halfWidth / 2), m_pos.y + (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, m_maxObjects, m_maxLevels);
	m_nodes[(int)BOTTOM_RIGHT] = new Quadtree(Vector3(m_pos.x + (m_halfWidth / 2), m_pos.y + (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, m_maxObjects, m_maxLevels);
}

//Clears away all objects and delete all children nodes
void Quadtree::Clear()
{
	//Set the current partition each object is part of to nullptr then clear the objects
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->SetCurrentPartition(nullptr);
	}
	m_objects.clear();
	//If has a child node
	if (m_nodes[0])
	{
		//Delete all child nodes which will clear the objects
		for (int i = 0; i < (int)MAX_CORNERS; ++i)
		{
			delete m_nodes[i];
			m_nodes[i] = nullptr;
		}
	}
}

Partition* Quadtree::FindPartition(Vector3 _pos, int _level)
{
	if (m_level < _level)
	{
		if (m_nodes[0])
		{
			int index = GetIndex(_pos);
			return m_nodes[index]->FindPartition(_pos, _level);
		}
	}
	return this;
}

void Quadtree::Rebuild()
{
	//Only try if can go down more levels
	if (m_level < m_maxLevels)
	{
		//Only if the object cap has been reached
		if ((int)m_objects.size() > m_maxObjects)
		{
			Split();
			for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
			{
				int index = GetIndex((*it)->GetGameObject()->GetPos());
				m_nodes[index]->AssignObject(*it);
			}
			m_objects.clear();
			for (int i = 0; i < (int)MAX_CORNERS; ++i)
			{
				m_nodes[i]->Rebuild();
			}
		}
	}
}

void Quadtree::FindTest(FindPointTest* _test)
{
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (PointQuery((*it)->GetGameObject()->GetPos(), _test->upperLeft, _test->lowerRight))
		{
			++_test->pointsFound;
		}
		++_test->numberPointChecks;
	}
	if (m_nodes[0])
	{
		for (int i = 0; i < (int)MAX_CORNERS; ++i)
		{
			if (m_nodes[i]->ShapeQuery(_test->upperLeft, _test->lowerRight))
			{
				m_nodes[i]->FindTest(_test);
			}
			++_test->numberNodeChecks;
		}
	}
	++_test->nodesTravelled;
}

list<PartitionObject*> Quadtree::CheckTest(CheckPointTest* _test)
{
	list<PartitionObject*> _checkObjects;
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (PointQuery((*it)->GetGameObject()->GetPos(), _test->upperLeft, _test->lowerRight))
		{
			_checkObjects.push_back(*it);
		}
	}
	if (m_nodes[0])
	{
		for (int i = 0; i < (int)MAX_CORNERS; ++i)
		{
			if (m_nodes[i]->ShapeQuery(_test->upperLeft, _test->lowerRight))
			{
				list<PartitionObject*> _temp = m_nodes[i]->CheckTest(_test);
				_checkObjects.insert(_checkObjects.end(), _temp.begin(), _temp.end());
			}
		}
	}
	return _checkObjects;
}

void Quadtree::Tick(GameData* _GD)
{
	_GD;
	//m_outline->Tick(_GD);
}

void Quadtree::Draw(DrawData* _DD)
{
	m_outline->Draw(_DD);
	if (m_nodes[0])
	{
		for (int i = 0; i < (int)MAX_CORNERS; ++i)
		{
			m_nodes[i]->Draw(_DD);
		}
	}
}

void Quadtree::HighlightObjects(Color _col)
{
	if (m_nodes[0])
	{
		for (int i = 0; i < (int)MAX_CORNERS; ++i)
		{
			m_nodes[i]->HighlightObjects(_col);
		}
	}
	Partition::HighlightObjects(_col);
}

void Quadtree::UnHighlightObjects()
{
	if (m_nodes[0])
	{
		for (int i = 0; i < (int)MAX_CORNERS; ++i)
		{
			m_nodes[i]->UnHighlightObjects();
		}
	}
	Partition::UnHighlightObjects();
}