#include "Quadtree.h"
#include "PartitionManager.h"
#include "PartitionObject.h"
#include "VBShape.h"

Quadtree::Quadtree(Vector3 _centre, float _halfWidth, int _level, Quadtree* _parent)
{
	m_pos = _centre;
	m_halfWidth = _halfWidth;
	m_level = _level;
	m_nodes.resize((int)MAX_CORNERS);
	m_parent = _parent;

	m_outline = new VBShape();
	m_outline->InitialiseShape("WireCube2D");
	m_outline->SetPos(m_pos);
	m_outline->SetScale(Vector3(_halfWidth,_halfWidth, 1));
	m_outline->Tick(nullptr);

	m_maxObjects = 10;
	m_maxLevels = 5;
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
	_object->s_partitionManager->GetCurrentRoot()->Insert(_object);
}

void Quadtree::Split()
{
	m_nodes[(int)TOP_RIGHT] = new Quadtree(Vector3(m_pos.x + (m_halfWidth / 2), m_pos.y - (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, this);
	m_nodes[(int)TOP_LEFT] = new Quadtree(Vector3(m_pos.x - (m_halfWidth / 2), m_pos.y - (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, this);
	m_nodes[(int)BOTTOM_LEFT] = new Quadtree(Vector3(m_pos.x - (m_halfWidth / 2), m_pos.y + (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, this);
	m_nodes[(int)BOTTOM_RIGHT] = new Quadtree(Vector3(m_pos.x + (m_halfWidth / 2), m_pos.y + (m_halfWidth / 2), 0.0f), m_halfWidth / 2, m_level + 1, this);
}

list<PartitionObject*> Quadtree::Retrieve(PartitionObject* _object)
{
	_object;
	list<PartitionObject*> retrieve;
	return retrieve;
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

void Quadtree::Rebuild(list<PartitionObject*> _objects)
{
	Clear();
	m_objects = _objects;
	Rebuild();
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