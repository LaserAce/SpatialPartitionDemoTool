#include "PartitionObject.h"
#include "Partition.h"
#include "PartitionManager.h"

PartitionObject::PartitionObject(GameObject* _gameObject)
{
	p_gameObject = _gameObject;
	it_managerLocation = PartitionManager::Singleton()->AddToManagerList(this);
}

PartitionObject::~PartitionObject()
{
	p_gameObject->SetPartitionObject(nullptr);
	PartitionManager::Singleton()->RemoveFromManagerList(it_managerLocation);
	if (p_partition)
	{
		Remove();
	}
}

void PartitionObject::Insert()
{
	PartitionManager::Singleton()->GetCurrentRoot()->Insert(this);
}

void PartitionObject::Update()
{
	if (p_partition)
	{
		p_partition->Update(this);
	}
}

void PartitionObject::Remove()
{
	if (p_partition)
	{
		p_partition->Remove(this);
	}
}


