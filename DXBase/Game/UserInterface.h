#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include <AntTweakBar.h>
#include "gameobject.h"

using namespace DirectX;
using namespace SimpleMath;

class PartitionManager;
class VBShape;

enum InterfaceMethod
{
	INTERFACE_POINT = 0,
	INTERFACE_QUERY,
	MAX_INTERFACE_METHODS
};

class UserInterface : public GameObject
{
public:
	UserInterface();
	virtual ~UserInterface();
	void SetupTwBars();
	void AdjustSize();

	static void TW_CALL RebuildPartition(void* _clientData);
	static void TW_CALL DeletePoints(void* _clientData);
	static void TW_CALL SetActiveMethod(const void *value, void *clientData);
	static void TW_CALL GetActiveMethod(void *value, void *clientData);
	static void TW_CALL SetInterfaceMethod(const void *value, void *clientData);
	static void TW_CALL GetInterfaceMethod(void *value, void *clientData);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	InterfaceMethod* GetInterfaceMehod(){ return &m_method; }
	void SetInterfaceMethod(InterfaceMethod _method){ m_method = _method; }

	void RemoveVariables(list<const char*>* _variables);

	//list<const char*>* GetInterfaceMethodVariables(){ return &m_interfaceMethodVariables; }
	void InterfacePointMethod();
protected:
	bool PointWithinBounds(Vector2 _topLeft, Vector2 _bottomRight, Vector2 _point);

	TwBar* leftUI;
	//lower number takes up more space on screen
	float m_size;

	InterfaceMethod m_method;
	
	void AddVarCB(TwBar* _bar, const char* _name, TwType _type, TwSetVarCallback _setCallback, TwGetVarCallback _getCallback, void* _clientData, const char* _def, list<const char*>* _list);
	void AddVarRW(TwBar* _bar, const char* _name, TwType _type, void* _var, const char* _def, list<const char*>* _list);

	list<const char*>  m_interfaceMethodVariables;

	int m_pointsToSpawn = 1;
	int m_pointsRange = 10;

	RECT m_winSize;
	VBShape* m_wireRadius;
};

#endif