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