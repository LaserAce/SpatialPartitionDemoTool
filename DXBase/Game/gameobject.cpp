//Base Game Object Class
#include "GameObject.h"
#include "PartitionObject.h"

using namespace std;

list<GameObject*>* GameObject::p_gameObjects;

GameObject::GameObject()
{
	//set the Gameobject to the origin with no rotation and unit scaling 
	m_pos = Vector3::Zero;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;
	m_scale = Vector3::One;

	m_parent = nullptr;

	m_worldMat = Matrix::Identity;
	m_fudge = Matrix::Identity;
}

GameObject::~GameObject()
{
	if (p_partitionObject)
	{
		delete p_partitionObject;
	}
}

void GameObject::Tick(GameData* _GD)
{
	_GD;
	//build up the world matrix depending on the new position of the GameObject
	Matrix  _scaleMat	= Matrix::CreateScale( m_scale);
	m_rotMat = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
	Matrix  _transMat = Matrix::CreateTranslation(m_pos);

	m_worldMat = _scaleMat *m_rotMat * _transMat;

}

void GameObject::NewPartitionObject()
{
	//Creates a new partition object and inserts into currently selected partition
	p_partitionObject = new PartitionObject(this);
	p_partitionObject->Insert();
}

void GameObject::InsertToList()
{
	//Allows the GameObject to be added to the main game list
	p_gameObjects->push_back(this);
	it_gameObjects = prev(p_gameObjects->end());
}

void GameObject::RemoveFromList()
{
	//Removes the GameObject directly from the main game list
	p_gameObjects->erase(it_gameObjects);
}



