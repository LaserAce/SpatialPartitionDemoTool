#include "KDtree.h"
#include "PartitionManager.h"
#include "PartitionObject.h"
#include "VBShape.h"

KDtree::KDtree(Vector3 _centre, Vector3 _extents, bool _split, int _level, KDtree* _parent)
{
	m_level = _level;
	m_parent = _parent;
	m_verticalSplit = _split;

	m_pos = _centre;
	m_extents = _extents;

	m_outline = new VBShape();
	m_outline->InitialiseShape("WireCube2D");
	m_outline->SetPos(m_pos);
	m_outline->SetScale(m_extents);
	m_outline->Tick(nullptr);

	m_maxObjects = 1;
	m_maxLevels = 100;
}

KDtree::~KDtree()
{
	Clear();
	delete m_outline;
}

KDtree* KDtree::GetIndex(Vector3 _pos)
{
	Vector3 pos = _pos;
	if (m_verticalSplit)
	{
		if (m_left_top->GetPos().x + m_left_top->GetExtents().x > pos.x)
		{
			return m_left_top;
		}
		else
		{
			return m_right_bottom;
		}
	}
	else
	{
		if (m_left_top->GetPos().y + m_left_top->GetExtents().y > pos.y)
		{
			return m_left_top;
		}
		else
		{
			return m_right_bottom;
		}
	}
}

void KDtree::Insert(PartitionObject* _object)
{
	//If a child node exists
	if (m_left_top)
	{
		KDtree* node = GetIndex(_object->GetGameObject()->GetPos());
		node->Insert(_object);
		return;
	}

	AssignObject(_object);

	//Only try if can go down more levels
	if (m_level < m_maxLevels)
	{
		//Only if the object cap has been reached
		if ((int)m_objects.size() > m_maxObjects)
		{
			if (!m_left_top)
			{
				Split();
			}

			//Move all objects down the tree
			for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end();)
			{
				KDtree* node = GetIndex((*it)->GetGameObject()->GetPos());
				node->Insert(*it);
				it = m_objects.erase(it);
			}
		}
	}
}

void KDtree::Remove(PartitionObject* _object)
{
	m_objects.erase(_object->GetPartitionIterator());
}

void KDtree::Update(PartitionObject* _object)
{
	Remove(_object);
	_object->s_partitionManager->GetCurrentRoot()->Insert(_object);
}

void KDtree::Split()
{
	float split = 0;
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (m_verticalSplit)
		{
			split += (*it)->GetGameObject()->GetPos().x;
		}
		else
		{
			split += (*it)->GetGameObject()->GetPos().y;
		}
	}
	split /= m_objects.size();
	if (m_verticalSplit)
	{
		Vector3 centre = Vector3(((m_pos.x - m_extents.x) + split) / 2, m_pos.y, 1.0f);
		Vector3 extents = Vector3(split - centre.x, m_extents.y, 1.0f);
		m_left_top = new KDtree(centre,extents,false, m_level+1, this);
		centre = Vector3(((m_pos.x + m_extents.x) + split) / 2, m_pos.y, 1.0f);
		extents = Vector3(centre.x - split, m_extents.y, 1.0f);
		m_right_bottom = new KDtree(centre, extents, false, m_level + 1, this);
	}
	else
	{
		Vector3 centre = Vector3(m_pos.x, ((m_pos.y - m_extents.y) + split) / 2, 1.0f);
		Vector3 extents = Vector3(m_extents.x, split - centre.y, 1.0f);
		m_left_top = new KDtree(centre, extents, true, m_level + 1, this);
		centre = Vector3(m_pos.x, ((m_pos.y + m_extents.y) + split) / 2, 1.0f);
		extents = Vector3(m_extents.x, centre.y - split, 1.0f);
		m_right_bottom = new KDtree(centre, extents, true, m_level + 1, this);
	}
}

list<PartitionObject*> KDtree::Retrieve(PartitionObject* _object)
{
	_object;
	list<PartitionObject*> retrieve;
	return retrieve;
}

//Clears away all objects and delete all children nodes
void KDtree::Clear()
{
	//Set the current partition each object is part of to nullptr then clear the objects
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->SetCurrentPartition(nullptr);
	}
	m_objects.clear();
	if (m_left_top)
	{
		delete m_left_top;
		m_left_top = nullptr;
		delete m_right_bottom;
		m_right_bottom = nullptr;
	}
}

Partition* KDtree::FindPartition(Vector3 _pos, int _level)
{
	if (m_level < _level)
	{
		if (m_left_top)
		{
			return GetIndex(_pos)->FindPartition(_pos, _level);
		}
	}
	return this;
}

void KDtree::Rebuild(list<PartitionObject*> _objects)
{
	Clear();
	m_objects = _objects;
	Rebuild();
}

void KDtree::Rebuild()
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
				KDtree* node = GetIndex((*it)->GetGameObject()->GetPos());
				node->AssignObject(*it);
			}
			m_objects.clear();
			m_left_top->Rebuild();
			m_right_bottom->Rebuild();
		}
	}
	
}



void KDtree::Tick(GameData* _GD)
{
	_GD;
	//m_outline->Tick(_GD);
}

void KDtree::Draw(DrawData* _DD)
{
	m_outline->Draw(_DD);
	if (m_left_top)
	{
		m_left_top->Draw(_DD);
		m_right_bottom->Draw(_DD);
	}
}

void KDtree::HighlightObjects(Color _col)
{
	if (m_left_top)
	{
		m_left_top->HighlightObjects(_col);
		m_right_bottom->HighlightObjects(_col);
	}
	Partition::HighlightObjects(_col);
}

void KDtree::UnHighlightObjects()
{
	if (m_left_top)
	{
		m_left_top->UnHighlightObjects();
		m_right_bottom->UnHighlightObjects();
	}
	Partition::UnHighlightObjects();
}