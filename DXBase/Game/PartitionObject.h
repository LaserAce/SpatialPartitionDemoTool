#ifndef _PARTITION_OBJECT_H_
#define _PARTITION_OBJECT_H_

#include <list>
using std::list;

class Partition;
class GameObject;
class PartitionManager;

class PartitionObject
{
public:
	PartitionObject(GameObject* _gameObject);
	~PartitionObject();

	void Insert();
	void Update();
	void Remove();

	void SetCurrentPartition(Partition* _partition) { p_partition = _partition; }

	list<PartitionObject*>::iterator GetPartitionIterator() { return it_partitionLocation; }
	void SetPartitionIterator(list<PartitionObject*>::iterator _it) { it_partitionLocation = _it; }

	GameObject* GetGameObject(){ return p_gameObject; }

protected:
private:
	Partition* p_partition;   //The partition that this object is currently located in
	list<PartitionObject*>::iterator it_partitionLocation;   //An iterator that is the location of the object in the partitions object list
	list<PartitionObject*>::iterator it_managerLocation;
	GameObject* p_gameObject;   //The gameobject that this object represents
};

#endif