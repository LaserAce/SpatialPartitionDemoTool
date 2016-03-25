#ifndef _KD_TREE_H_
#define _KD_TREE_H_

#include "Partition.h"
#include <vector>

using std::vector;

class KDtree : public Partition
{
public:
	KDtree(Vector3 _centre, Vector3 _extents, bool _split, int _level, int _maxObjects, int _maxLevels);
	~KDtree();
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

	Vector3 GetExtents(){ return m_extents; }

protected:
private:
	KDtree* m_left_top = nullptr;
	KDtree* m_right_bottom = nullptr;

	bool m_verticalSplit;

	Vector3 m_extents;

	void Split();

	KDtree* GetIndex(Vector3 _pos);
};

#endif