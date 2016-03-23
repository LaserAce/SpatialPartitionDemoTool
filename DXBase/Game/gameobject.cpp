#include "gameobject.h"
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
	//the assumption is that this class will be inherited by the class that ACTUALLY changes this
	Matrix  scaleMat	= Matrix::CreateScale( m_scale);
	m_rotMat = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);//possible not the best way of doing this!
	Matrix  transMat = Matrix::CreateTranslation(m_pos);

	m_worldMat = scaleMat *m_rotMat * transMat;

}

void GameObject::InsertToList()
{
	p_gameObjects->push_back(this);
	it_gameObjects = prev(p_gameObjects->end());
}

void GameObject::RemoveFromList()
{
	p_gameObjects->erase(it_gameObjects);
}

void GameObject::NewPartitionObject()
{
	p_partitionObject = new PartitionObject(this);
	p_partitionObject->Insert();
}

