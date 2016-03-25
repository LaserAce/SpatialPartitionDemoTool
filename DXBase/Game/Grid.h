#ifndef _GRID_H_
#define _GRID_H_

#include "Partition.h"
#include <vector>

using std::vector;

class Grid : public Partition
{
public:
	Grid(Vector3 _centre, Vector3 _extents, int _splits, int _level, int _maxObjects, int _maxLevels);
	~Grid();
	virtual void Insert(PartitionObject* _object);
	virtual void Remove(PartitionObject* _object);
	virtual void Update(PartitionObject* _object);
	virtual void Clear();
	virtual Partition* FindPartition(Vector3 _pos, int _level);
	virtual void Rebuild();
	virtual void FindTest(FindPointTest* _test);
	virtual list<PartitionObject*> CheckTest(CheckPointTest* _test);

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