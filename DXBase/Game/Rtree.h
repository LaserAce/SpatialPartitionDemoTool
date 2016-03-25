#ifndef _R_TREE_H_
#define _R_TREE_H_

#include "Partition.h"

class Rtree : public Partition
{
public:
	Rtree();
	~Rtree();
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
protected:
private:
};

#endif