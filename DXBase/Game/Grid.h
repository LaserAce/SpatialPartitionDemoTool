#ifndef _GRID_H_
#define _GRID_H_

#include "Partition.h"
#include <vector>

using std::vector;

class Grid : public Partition
{
public:
	Grid(Vector3 _centre, Vector3 _extents, int _splits, int _level, Grid* _parent);
	~Grid();
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
	vector<Grid*> m_grids;
	int m_splits;
	Vector3 m_extents;

	void Split();

	int GetIndex(Vector3 _pos);
};

#endif