#include "Partition.h"
#include "PartitionObject.h"
#include "VBShape.h"

void Partition::HighlightObjects(Color _col)
{
	m_outline->SetColour(_col);
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		dynamic_cast<VBShape*>((*it)->GetGameObject())->SetColour(_col);
	}
}

void Partition::UnHighlightObjects()
{
	m_outline->SetToDefaultColour();
	for (list<PartitionObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		dynamic_cast<VBShape*>((*it)->GetGameObject())->SetToDefaultColour();
	}
}

void Partition::AssignObject(PartitionObject* _object)
{
	_object->SetCurrentPartition(this);
	m_objects.push_back(_object);
	_object->SetPartitionIterator(prev(m_objects.end()));
}

void Partition::Reset()
{
	m_maxObjects = m_defaultMaxObjects;
	m_maxLevels = m_defaultMaxLevels;
}

bool Partition::PointQuery(Vector3 _pos, Vector2 _upperLeft, Vector2 _lowerRight)
{
	if (_pos.x <= _lowerRight.x)
	{
		if (_pos.x >= _upperLeft.x)
		{
			if (_pos.y >= _lowerRight.y)
			{
				if (_pos.y <= _upperLeft.y)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Partition::ShapeQuery(Vector2 _upperLeft, Vector2 _lowerRight)
{
	if (m_upperLeft.x > _lowerRight.x)
	{
		return false;
	}
	if (m_lowerRight.x < _upperLeft.x)
	{
		return false;
	}
	if (m_upperLeft.y < _lowerRight.y)
	{
		return false;
	}
	if (m_lowerRight.y > _upperLeft.y)
	{
		return false;
	}
	return true;;
}