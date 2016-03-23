#ifndef _BRUTE_FORCE_H_
#define _BRUTE_FORCE_H_

#include "Partition.h"

class BruteForce : public Partition
{
public:
	BruteForce();
	~BruteForce();
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
protected:
private:
};

#endif