//Spatial Partitioning
#ifndef _SPACIAL_PARTITION_H_
#define _SPACIAL_PARTITION_H_

#include <list>
#include <unordered_map>
#include <vector>
#include <string>
#include "SimpleMath.h"

using std::list;
using std::unordered_map;
using std::pair;
using std::string;
using std::vector;

using namespace DirectX;
using namespace SimpleMath;

class Collision;
class VBShape;

struct PartitionCell
{
	Vector2 m_index;   //The cell index that represents the top left corner of a cell
	list<Collision*> objects;   //The list of objects in a particular cell
	vector<PartitionCell*> neighbours;   //A vector containing the neighbouring cells
	VBShape* debugShape = nullptr;
};

class SpatialPartition
{
public:
	SpatialPartition();
	~SpatialPartition();
	//Try and add a new collision to a cell
	void Insert(Collision* newObject);
	//Update an object and determines if it needs to move cells
	void Update(Collision* updateObject);
	//Remove a collision from a cell
	void Remove(Collision* removeObject);
	//Retreives all objects associated with collisionobject and passes them on
	list<Collision*> Retrieve(Collision* retrieveObject);
	//Get all cells
	unordered_map<string, PartitionCell*> GetCells(){ return m_cells; }


protected:

	//Will determine the associated int key with an index
	string GetIntegerKey(Vector2 index);
	//Will determine the correct cell an object should be in
	Vector2 GetIndex(Collision* indexObject);
	//Check whether a paticular cell should be delted and deletes it
	void DeleteCheck(PartitionCell* deleteCell);
	//Get neighbours and create connections
	void SetNeighbours(PartitionCell* centreCell);

	unordered_map<string, PartitionCell*> m_cells;   //Unordered map is more efficient for getting items based on keys as opposed to iterating through

	const float CELLDISTANCE = 15;

};
#endif