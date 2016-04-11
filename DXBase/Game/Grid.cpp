#include "Grid.h"
#include "PartitionManager.h"
#include "PartitionObject.h"
#include "VBShape.h"
#include "Test.h"

Grid::Grid(Vector3 _centre, Vector3 _extents, int _splits, int _level, int _maxObjects, int _maxLevels)
{
	m_defaultMaxObjects = 0;
	m_defaultMaxLevels = 1;

	m_maxLevels = _maxLevels;
	m_maxObjects = _maxObjects;

	m_level = _level;
	m_splits = _splits;

	m_pos = _centre;
	m_extents = _extents;

	m_outline = new VBShape();
	m_outline->InitialiseShape("WireCube2D");
	m_outline->SetPos(m_pos);
	m_outline->SetScale(m_extents);
	m_outline->Tick(nullptr);

	m_upperLeft = Vector2(_centre.x - _extents.x, _centre.y + _extents.y);
	m_lowerRight = Vector2(_centre.x + _extents.x, _centre.y - _extents.y);
}

Grid::~Grid()
{
	Clear();
	delete m_outline;
}

int Grid::GetIndex(Vector3 _pos)
{
	Vector3 pos = _pos;

	float multiple = (m_extents.x / ((float)m_splits / 2));

	float modifier = 0;
	if (m_splits % 2 == 0)
	{
		modifier = multiple / 2;
		/*if (pos.x > 0)
		{
			modifier *= -1;
		}*/
	}

	float x = pos.x + (multiple / 2) + modifier;
	if (x < 0)
	{
		x -= multiple + fmodf(x, multiple);
	}
	else
	{
		x -= fmodf(x, multiple);
	}
	x -= modifier;

	if (x > m_extents.x)
	{
		x -= multiple;
	}
	else if(x < -m_extents.x)
	{
		x += multiple;
	}

	x += m_extents.x;
	x /= multiple;
	x = floor(x);

	multiple = (m_extents.y / ((float)m_splits / 2));

	modifier = 0;
	if (m_splits % 2 == 0)
	{
		modifier = multiple / 2;
		/*if (pos.y > 0)
		{
			modifier *= -1;
		}*/
	}

	float y = pos.y + (multiple / 2) + modifier;
	if (y < 0)
	{
		y -= multiple + fmodf(y, multiple);
	}
	else
	{
		y -= fmodf(y, multiple);
	}
	y -= modifier;

	if (y > m_extents.y)
	{
		y -= multiple;
	}
	else if (y < -m_extents.y)
	{
		y += multiple;
	}

	y += m_extents.y;
	y /= multiple;
	y = floor(y);

	return (int)((y * m_splits) + x);
}

void Grid::Insert(PartitionObject* _object)
{
	//If a child node exists
	if (!m_grids.empty())
	{
		int index = GetIndex(_object->GetGameObject()->GetPos());
		m_grids[index]->Insert(_object);
		return;
	}

	AssignObject(_object);

	//Only try if can go down more levels
	if (m_level < m_maxLevels)
	{
		//Only if the object cap has been reached
		if ((int)m_objects.size() > m_maxObjects)
		{
			if (m_grids.empty())
			{
				Split();
			}

			//Move all objects down the tree
			for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end();)
			{
				int index = GetIndex((*it)->GetGameObject()->GetPos());
				m_grids[index]->Insert(*it);
				it = m_objects.erase(it);
			}
		}
	}
}

void Grid::Remove(PartitionObject* _object)
{
	m_objects.erase(_object->GetPartitionIterator());
	_object->SetCurrentPartition(nullptr);
}

void Grid::Update(PartitionObject* _object)
{
	Remove(_object);
	PartitionManager::Singleton()->GetCurrentRoot()->Insert(_object);
}

void Grid::Split()
{
	//Split equally in both direction
	for (int i = 0; i < m_splits; ++i)
	{
		for (int j = 0; j < m_splits; ++j)
		{
			//Create position of node
			float y = (i * (m_extents.y / ((float)m_splits / 2))) + (m_extents.y / (float)m_splits) - m_extents.y;
			float x = (j * (m_extents.x / ((float)m_splits / 2))) + (m_extents.x / (float)m_splits) - m_extents.x;
			//Create extents of node
			Vector3 extents = Vector3(m_extents.x / (float)m_splits, m_extents.y / (float)m_splits, m_extents.z);
			//Create node using position and extents
			m_grids.push_back(new Grid(Vector3(x + m_pos.x,y + m_pos.y,0.0f),extents,m_splits,m_level+1, m_maxObjects, m_maxLevels));
		}
	}
}

//Clears away all objects and delete all children nodes
void Grid::Clear()
{
	//Set the current partition each object is part of to nullptr then clear the objects
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->SetCurrentPartition(nullptr);
	}
	m_objects.clear();
	for (int i = 0; i < (int)m_grids.size(); ++i)
	{
		delete m_grids[i];
	}
	m_grids.clear();
}

Partition* Grid::FindPartition(Vector3 _pos, int _level)
{
	if (m_level < _level)
	{
		if (!m_grids.empty())
		{
			int index = GetIndex(_pos);
			if (index >= (int)m_grids.size() || index < 0)
			{
				return this;
			}
			return m_grids[index]->FindPartition(_pos, _level);
		}
	}
	return this;
}

void Grid::Rebuild()
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
				m_grids[index]->AssignObject(*it);
			}
			m_objects.clear();
			for (int i = 0; i < (int)m_grids.size(); ++i)
			{
				m_grids[i]->Rebuild();
			}
		}
	}

}

void Grid::FindTest(FindPointTest* _test)
{
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (PointQuery((*it)->GetGameObject()->GetPos(), _test->upperLeft, _test->lowerRight))
		{
			++_test->pointsFound;
		}
		++_test->numberPointChecks;
	}
	for (int i = 0; i < (int)m_grids.size(); ++i)
	{
		if (m_grids[i]->ShapeQuery(_test->upperLeft, _test->lowerRight))
		{
			m_grids[i]->FindTest(_test);
		}
		++_test->numberNodeChecks;
	}
	++_test->nodesTravelled;
}

list<PartitionObject*> Grid::CheckTest(CheckPointTest* _test)
{
	list<PartitionObject*> _checkObjects;
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (PointQuery((*it)->GetGameObject()->GetPos(), _test->upperLeft, _test->lowerRight))
		{
			_checkObjects.push_back(*it);
		}
	}
	for (int i = 0; i < (int)m_grids.size(); ++i)
	{
		if (m_grids[i]->ShapeQuery(_test->upperLeft, _test->lowerRight))
		{
			list<PartitionObject*> _temp = m_grids[i]->CheckTest(_test);
			_checkObjects.insert(_checkObjects.end(), _temp.begin(), _temp.end());
		}
	}
	return _checkObjects;
}


void Grid::Tick(GameData* _GD)
{
	_GD;
	//m_outline->Tick(_GD);
}

void Grid::Draw(DrawData* _DD)
{
	m_outline->Draw(_DD);
	for (int i = 0; i < (int)m_grids.size(); ++i)
	{
		m_grids[i]->Draw(_DD);
	}
}

void Grid::HighlightObjects(Color _col)
{
	for (int i = 0; i < (int)m_grids.size(); ++i)
	{
		m_grids[i]->HighlightObjects(_col);
	}
	Partition::HighlightObjects(_col);
}

void Grid::UnHighlightObjects()
{
	for (int i = 0; i < (int)m_grids.size(); ++i)
	{
		m_grids[i]->UnHighlightObjects();
	}
	Partition::UnHighlightObjects();
}