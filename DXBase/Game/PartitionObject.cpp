#include "PartitionObject.h"
#include "Partition.h"
#include "PartitionManager.h"

PartitionManager* PartitionObject::s_partitionManager = nullptr;

PartitionObject::PartitionObject(GameObject* _gameObject)
{
	p_gameObject = _gameObject;
	it_managerLocation = s_partitionManager->AddToManagerList(this);
}

PartitionObject::~PartitionObject()
{
	p_gameObject->SetPartitionObject(nullptr);
	s_partitionManager->RemoveFromManagerList(it_managerLocation);
	Remove();
}

void PartitionObject::Insert()
{
	s_partitionManager->GetCurrentRoot()->Insert(this);
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


