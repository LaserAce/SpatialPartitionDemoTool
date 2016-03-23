#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Partition.h"
#include <vector>

using std::vector;

enum QuadCorner
{
	TOP_RIGHT = 0,
	TOP_LEFT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	MAX_CORNERS
};

class Quadtree : public Partition
{
public:
	Quadtree(Vector3 _centre, float _halfWidth, int _level, Quadtree* _parent);
	~Quadtree();
	virtual void Insert(PartitionObject* _object);
	virtual void Remove(PartitionObject* _object);
	virtual void Update(PartitionObject* _object);
	virtual list<PartitionObject*> Retrieve(PartitionObject* _object);
	virtual void Clear();
	virtual Partition* FindPartition(Vector3 _pos, int _level);
	virtual void Rebuild(list<PartitionObject*> _objects);
	virtual void Rebuild();
	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);
	
	virtual void HighlightObjects(Color _col);
	virtual void UnHighlightObjects();
protected:
private:
	vector<Quadtree*> m_nodes;   //An array of size 4 that holds the child partitions
	float m_halfWidth;
	void Split();
	int GetIndex(Vector3 _pos);
};

#endif