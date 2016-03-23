#include "spatialpartition.h"
#include "collision.h"
#include "VBShape.h"
#include "staticdata.h"

using std::to_string;

SpatialPartition::SpatialPartition()
{

}

SpatialPartition::~SpatialPartition()
{
	//Delete all the cells
	for (unordered_map<string, PartitionCell*>::iterator it = m_cells.begin(); it != m_cells.end(); it++)
	{
		delete it->second;
	}
	m_cells.clear();
}

Vector2 SpatialPartition::GetIndex(Collision* _object)
{
	//Make an index from a position by flooring to an integer and rounding
	Vector2 index;
	index.x = std::floor(_object->GetPos().x / CELLDISTANCE) * CELLDISTANCE;
	index.y = std::floor(_object->GetPos().z / CELLDISTANCE) * CELLDISTANCE;
	return index;
}

string SpatialPartition::GetIntegerKey(Vector2 _index)
{
	//Converts the index into integer that is made from x followed by y
	string key;
	key = to_string(static_cast<int>(_index.x)) + to_string(static_cast<int>(_index.y));
	return key;
}

void SpatialPartition::Insert(Collision* _object)
{
	//Determine what cell the collision should be in
	Vector2 index = GetIndex(_object);
	string key = GetIntegerKey(index);
	unordered_map<string, PartitionCell*>::iterator it = m_cells.find(key);

	//If the cell doesn't exist
	if (it == m_cells.end())
	{
		//Creating a new cell
		PartitionCell* newCell = new PartitionCell;
		newCell->m_index = index;
		m_cells.insert(std::make_pair(key, newCell));
		SetNeighbours(newCell);

		//While in debug create shapes to visual see collision cells
#if DEBUG
		VBShape* shape = new VBShape();
		shape->SetPos(Vector3(index.x + (CELLDISTANCE / 2), 0, index.y + (CELLDISTANCE / 2)));
		shape->SetScale(CELLDISTANCE/2);
		shape->ChangeColor(Color(0, 1, 0));
		shape->InitialiseShape("VBCube3");
		shape->SetWireframe(true, StaticData::s_pd3dDevice);
		newCell->debugShape = shape;
#endif

		it = m_cells.find(key);
	}
	//Push object into cell and create connection
	it->second->objects.push_back(_object);
	_object->ptrCell = it->second;
}

void SpatialPartition::Update(Collision* _object)
{
	//If an object is not connected to a cell
	if (_object->ptrCell == nullptr)
	{
		Insert(_object);
		return;
	}
	Vector2 index = GetIndex(_object);
	Vector2 currentIndex = _object->ptrCell->m_index;
	//Only do if the object needs to change cells
	if (currentIndex != index)
	{
		Remove(_object);
		Insert(_object);
	}
}

void SpatialPartition::Remove(Collision* _object)
{
	PartitionCell* cell = _object->ptrCell;
	//will remove a collision from a given cell
	for (list<Collision*>::iterator it = cell->objects.begin(); it != cell->objects.end(); it++)
	{
		if ((*it) == _object)
		{
			cell->objects.erase(it);
			break;
		}
	}
	//If cell has become empty then I will delete cell
	DeleteCheck(cell);
}

void SpatialPartition::DeleteCheck(PartitionCell* _cell)
{
	//If cell ha sno collisions stored
	if (_cell->objects.empty())
	{
		//Find the cell within the map and delete it
		unordered_map<string, PartitionCell*>::iterator it;
		string key = GetIntegerKey(_cell->m_index);
		it = m_cells.find(key);
		m_cells.erase(it);
		//Loop thorugh all cell neighbours
		for (vector<PartitionCell*>::iterator it = _cell->neighbours.begin(); it != _cell->neighbours.end(); ++it)
		{
			//Find this cell within neighbours neighbours
			for (vector<PartitionCell*>::iterator it2 = (*it)->neighbours.begin(); it2 != (*it)->neighbours.end(); ++it2)
			{
				if (_cell == (*it2))
				{
					(*it)->neighbours.erase(it2);
					break;
				}
			}
		}
		//While in debug delete shape afterwards
#if DEBUG
		delete _cell->debugShape;
#endif
		delete _cell;
	}
}

list<Collision*> SpatialPartition::Retrieve(Collision* _object)
{
	list<Collision*> retrieveList;
	
	//Retrieve all cell and neighbouring cell collisions
	PartitionCell* cell = _object->ptrCell;
	retrieveList.insert(retrieveList.end(), cell->objects.begin(), cell->objects.end());
	for (vector<PartitionCell*>::iterator it = cell->neighbours.begin(); it != cell->neighbours.end(); ++it)
	{
		retrieveList.insert(retrieveList.end(), (*it)->objects.begin(), (*it)->objects.end());
	}

	return retrieveList;
}

void SpatialPartition::SetNeighbours(PartitionCell* _centreCell)
{
	//Loop through potential neighbour positions
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			//If myself continue
			if (i == 0 && j == 0)
			{
				continue;
			}

			//Get potential neighbour keys
			unordered_map<string, PartitionCell*>::iterator it;
			string key = GetIntegerKey(Vector2(_centreCell->m_index.x + (i * CELLDISTANCE), _centreCell->m_index.y + (j * CELLDISTANCE)));
			it = m_cells.find(key);
			//Only if this neighbour cell exists
			if (it != m_cells.end())
			{
				_centreCell->neighbours.push_back((*it).second);
				(*it).second->neighbours.push_back(_centreCell);
			}
		}
	}
}



