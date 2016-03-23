#include "PartitionManager.h"
#include "Quadtree.h"
#include "KDtree.h"
#include "Grid.h"
#include "BruteForce.h"
#include "PartitionObject.h"
#include "pointer.h"

#include "gamedata.h"

PartitionManager* PartitionManager::singleton = nullptr;

PartitionManager::PartitionManager()
{
	singleton = this;

	p_highlightedPartition = nullptr;
	m_viewLevel = 0;
	m_debugVisible = true;

	m_activeMethod = GRID;
	m_partitionMethods.push_back(new BruteForce());
	m_partitionMethods.push_back(new Grid(Vector3(), Vector3(200, 200, 1), 10, 0, nullptr));
	m_partitionMethods.push_back(new Quadtree(Vector3(), 200, 0, nullptr));
	m_partitionMethods.push_back(new KDtree(Vector3(), Vector3(200,200, 1), true, 0, nullptr));
}

PartitionManager::~PartitionManager()
{
	for (int i = 0; i < (int)m_partitionMethods.size(); ++i)
	{
		delete m_partitionMethods[i];
	}
}

Partition* PartitionManager::GetCurrentRoot()
{
	return m_partitionMethods[(int)m_activeMethod];
}

list<PartitionObject*>::iterator PartitionManager::AddToManagerList(PartitionObject* _object)
{
	p_allPartitionObjects.push_back(_object);
	return prev(p_allPartitionObjects.end());
}

void PartitionManager::RemoveFromManagerList(list<PartitionObject*>::iterator _iterator)
{
	p_allPartitionObjects.erase(_iterator);
}

void PartitionManager::Tick(GameData* _GD)
{
	if (_GD->keyboard[DIK_SPACE] & 0x80)
	{
		RebuildPartition();
	}
	HighlightPartition();
	m_partitionMethods[(int)m_activeMethod]->Tick(_GD);
}

void PartitionManager::Draw(DrawData* _DD)
{
	if (m_debugVisible)
	{
		m_partitionMethods[(int)m_activeMethod]->Draw(_DD);
		if (p_highlightedPartition)
		{
			p_highlightedPartition->Draw(_DD);
		}
	}
}

void PartitionManager::HighlightPartition()
{
	Partition* p = m_partitionMethods[(int)m_activeMethod]->FindPartition(Pointer::Singleton()->GetPos(),m_viewLevel);
	if (p != p_highlightedPartition)
	{
		if (p)
		{
			if (p_highlightedPartition)
			{
				p_highlightedPartition->UnHighlightObjects();
			}
			p->HighlightObjects(Color(2, 0.2, 2));
		}
		p_highlightedPartition = p;
	}
}

void PartitionManager::UnHighlightPartition()
{
	if (p_highlightedPartition)
	{
		p_highlightedPartition->UnHighlightObjects();
	}
	p_highlightedPartition = nullptr;
}

void PartitionManager::RebuildPartition()
{
	UnHighlightPartition();
	m_partitionMethods[(int)m_activeMethod]->Rebuild(p_allPartitionObjects);
}

void PartitionManager::DeletePoints()
{
	UnHighlightPartition();
	m_partitionMethods[(int)m_activeMethod]->Clear();
	for (list<PartitionObject*>::iterator it = p_allPartitionObjects.begin(); it != p_allPartitionObjects.end(); )
	{
		(*it)->GetGameObject()->RemoveFromList();
		delete (*it)->GetGameObject();
		it = p_allPartitionObjects.begin();
	}
	p_allPartitionObjects.clear();
}

void PartitionManager::SetActiveMethod(PartitionMethods _method)
{
	UnHighlightPartition();
	m_partitionMethods[(int)m_activeMethod]->Clear();
	m_activeMethod = _method;
	RebuildPartition();
}

PartitionMethods* PartitionManager::GetActiveMethod()
{
	return &m_activeMethod;
}

void PartitionManager::SetViewLevel(int _i)
{
	UnHighlightPartition();
	m_viewLevel = _i;
}

int* PartitionManager::GetViewLevel()
{
	return &m_viewLevel;
}