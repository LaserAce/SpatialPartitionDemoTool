#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H
//Base Game Object Class
#include "SimpleMath.h"
#include "CommonStates.h"
#include <list>

using std::list;

using namespace DirectX;
using namespace SimpleMath;

struct GameData;
struct DrawData;
class PartitionObject;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD) = 0;

	Vector3 GetPos(){ return m_pos; }

	float GetPitch(){ return m_pitch; }
	float GetYaw(){ return m_yaw; }
	float GetRoll(){ return m_roll; }

	Vector3 GetScale(){ return m_scale; }

	void SetPos(Vector3 _pos){ m_pos = _pos; }

	void SetPitch(float _pitch){ m_pitch = _pitch; }
	void SetYaw(float _yaw){ m_yaw = _yaw; }
	void SetRoll(float _roll){ m_roll = _roll; }
	void SetPitchYawRoll(float _pitch, float _yaw, float _roll){ m_pitch = _pitch; m_yaw = _yaw; m_roll = _roll; }

	void SetScale(Vector3 _scale){ m_scale = _scale; }
	void SetScale(float _scale){ m_scale = _scale * Vector3::One; }

	Matrix GetWorldMat(){ return m_worldMat; }
	Matrix GetRotMat(){ return m_rotMat; }

	void SetWorldMat(Matrix _worldMat){ m_worldMat = _worldMat; }
	void SetRotMat(Matrix _rotMat){ m_rotMat = _rotMat; }

	GameObject* GetParent(){ return m_parent; }
	void SetParent(GameObject* _parent) { m_parent = _parent; }

	PartitionObject* GetPartitionObject(){ return p_partitionObject; }
	void SetPartitionObject(PartitionObject* _p){ p_partitionObject = _p; }
	void NewPartitionObject();

	static list<GameObject*>* p_gameObjects;
	void RemoveFromList();
	void InsertToList();

protected:
	Vector3 m_pos;
	float m_pitch, m_yaw, m_roll;
	Vector3 m_scale;

	PartitionObject* p_partitionObject = nullptr; 

	GameObject* m_parent = nullptr;
	Matrix m_worldMat;
	Matrix m_rotMat;
	Matrix m_fudge;

	list<GameObject*>::iterator it_gameObjects;

};

#endif