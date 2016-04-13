#include "PartitionManager.h"
#include "Quadtree.h"
#include "KDtree.h"
#include "Grid.h"
#include "BruteForce.h"
#include "Rtree.h"
#include "PartitionObject.h"
#include "Pointer.h"
#include "Test.h"
#include "TestManager.h"

#include "gamedata.h"

PartitionManager* PartitionManager::singleton = nullptr;

PartitionManager::PartitionManager()
{
	singleton = this;

	p_highlightedPartition = nullptr;
	m_viewLevel = 1;
	m_debugVisible = true;
	m_highlight = true;

	m_activeMethod = GRID;
	m_partitionMethods.push_back(new BruteForce());
	m_partitionMethods.push_back(new Grid(Vector3(), Vector3(200, 200, 1), 10, 0, 0, 1));
	m_partitionMethods.push_back(new Quadtree(Vector3(), 200, 0, 10, 5));
	m_partitionMethods.push_back(new KDtree(Vector3(), Vector3(200,200, 1), true, 0, 10, 50));
	m_partitionMethods.push_back(new Rtree());
}

PartitionManager::~PartitionManager()
{
	DeletePoints();
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
	if (!m_highlight)
	{
		return;
	}
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
	if (!m_highlight)
	{
		return;
	}
	if (p_highlightedPartition)
	{
		p_highlightedPartition->UnHighlightObjects();
	}
	p_highlightedPartition = nullptr;
}

void PartitionManager::RebuildPartition()
{
	UnHighlightPartition();
	TestManager* _man = TestManager::Singleton();
	if (*_man->GetBuildTest())
	{
		Test* _test = _man->BeginBuildTest();
		m_partitionMethods[(int)m_activeMethod]->Rebuild(p_allPartitionObjects);
		_man->EndBuildTest(_test);
	}
	else
	{
		m_partitionMethods[(int)m_activeMethod]->Rebuild(p_allPartitionObjects);
	}
}

void PartitionManager::ResetPartition()
{
	m_partitionMethods[(int)m_activeMethod]->Reset();
	RebuildPartition();
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

FindPointTest* PartitionManager::FindTest(FindPointTest* _test)
{
	m_partitionMethods[(int)m_activeMethod]->FindTest(_test);
	return _test;
}

CheckPointTest* PartitionManager::CheckTest(CheckPointTest* _test)
{
	list<PartitionObject*> _checkPoints = m_partitionMethods[(int)m_activeMethod]->CheckTest(_test);
	ULONGLONG timeAtBegin = GetTickCount64();
	for (list<PartitionObject*>::iterator it = _checkPoints.begin(); it != _checkPoints.end(); ++it)
	{
		for (list<PartitionObject*>::iterator it2 = next(it); it2 != _checkPoints.end(); ++it2)
		{
			++_test->numberPointToPointTests;
		}
	}
	_test->timeTaken = (int)(GetTickCount64() - timeAtBegin);
	return _test;
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

string PartitionManager::GetCurrentPartitionName()
{
	switch (m_activeMethod)
	{
	case NONE:
		return "No Method";
	case GRID:
		return "Grid";
	case QUADTREE:
		return "Quadtree";
	case KD_TREE:
		return "KDtree";
	case R_TREE:
		return "Rtree";
	default:
		return "Error: Method Not Found";
	}
}