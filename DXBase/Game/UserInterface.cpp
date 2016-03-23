#include "UserInterface.h"
#include "PartitionManager.h"
#include "gamedata.h"
#include <windows.h>
#include "VBShape.h"

UserInterface::UserInterface(PartitionManager* _man, GameObject* _pointer)
{
	p_partitionManager = _man;
	p_pointer = _pointer;
	m_method = INTERFACE_POINT;
	m_wireRadius = new VBShape();
	m_wireRadius->InitialiseShape("WireCircle2D");
	m_wireRadius->SetScale(0);
}

UserInterface::~UserInterface()
{
	TwDeleteAllBars();
	delete m_wireRadius;
}

void UserInterface::SetupTwBars()
{
	leftUI = TwNewBar("leftUI");
	TwDefine(" leftUI label='UI' ");
	TwDefine(" leftUI movable=false ");
	TwDefine(" leftUI resizable=false ");
	TwDefine(" leftUI color='122 122 122' ");
	TwDefine(" leftUI position='0 0' ");
	TwDefine(" leftUI iconifiable=false ");

	AdjustSize();

	//Define a tweak bar enumerator based on an actual enumerator
	TwEnumVal methods[MAX_METHODS] =
	{
		{ NONE, "Not Active" },
		{ GRID, "Grid" },
		{ QUADTREE, "Quadtree" },
		{ KD_TREE, "KD-tree" },
		{ R_TREE, "R-tree" },
	};
	TwType methodType = TwDefineEnum("MethodType", methods, MAX_METHODS);
	TwAddVarCB(leftUI, "Method", methodType, SetActiveMethod, GetActiveMethod, p_partitionManager, " label='Partitioning Method' ");
	TwAddVarRW(leftUI, "ViewLevel", TW_TYPE_INT32, p_partitionManager->GetViewLevel(), " min=0 max=100 label='View Level' ");
	TwAddVarRW(leftUI, "DebugVisible", TW_TYPE_BOOLCPP, p_partitionManager->GetDebugVisible(), " label='Partitions Visible' ");

	TwEnumVal interfaceMethods[MAX_INTERFACE_METHODS] =
	{
		{ INTERFACE_POINT, "Point" },
		{ INTERFACE_QUERY, "Query" }
	};
	TwType interfaceMethodType = TwDefineEnum("InterfaceMethodType", interfaceMethods, MAX_INTERFACE_METHODS);
	TwAddVarCB(leftUI, "InterfaceMethod", interfaceMethodType, SetInterfaceMethod, GetInterfaceMethod, this, " label='Mode' ");
	InterfacePointMethod();
}

void UserInterface::AdjustSize()
{
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &m_winSize);

	m_size = 0.15;
	int barSize[2] = { (int)((float)m_winSize.right * m_size), m_winSize.bottom };
	TwSetParam(leftUI, NULL, "size", TW_PARAM_INT32, 2, barSize);
}

void TW_CALL UserInterface::SetActiveMethod(const void *value, void *clientData)
{
	PartitionManager* man = static_cast<PartitionManager*>(clientData);
	man->SetActiveMethod(*static_cast<const PartitionMethods*>(value));
}

void  TW_CALL UserInterface::GetActiveMethod(void *value, void *clientData)
{
	PartitionManager* man = static_cast<PartitionManager*>(clientData);
	*(PartitionMethods*)value = *man->GetActiveMethod();
}

void TW_CALL UserInterface::SetInterfaceMethod(const void *value, void *clientData)
{
	UserInterface* ui = static_cast<UserInterface*>(clientData);
	ui->SetInterfaceMethod(*static_cast<const InterfaceMethod*>(value));
	ui->RemoveVariables(&ui->m_interfaceMethodVariables);
	switch (ui->m_method)
	{
	case INTERFACE_POINT:
		ui->InterfacePointMethod();
		break;
	case INTERFACE_QUERY:
		break;
	}
}

void  TW_CALL UserInterface::GetInterfaceMethod(void *value, void *clientData)
{
	UserInterface* ui = static_cast<UserInterface*>(clientData);
	*(InterfaceMethod*)value = *ui->GetInterfaceMehod();
}

void UserInterface::Tick(GameData* _GD)
{
	if (_GD->mouse->rgbButtons[0] && !_GD->prevMouse->rgbButtons[0])
	{
		POINT cursor;
		GetCursorPos(&(cursor));
		const HWND hDesktop = GetDesktopWindow();
		ScreenToClient(hDesktop, &cursor);

		if (!PointWithinBounds(Vector2(0.0f, 0.0f), Vector2((float)m_winSize.right * m_size, (float)m_winSize.bottom), Vector2((float)cursor.x, (float)cursor.y)))
		{
			p_partitionManager->UnHighlightPartition();
			if (m_pointsToSpawn == 1)
			{
				VBShape* p = new VBShape();
				p->InitialiseShape("WireDiamond2D");
				p->SetScale(0.75);
				p->SetPos(p_pointer->GetPos());
				p->NewPartitionObject();
				p->InsertToList();
			}
			else
			{
				for (int i = 0; i < m_pointsToSpawn; ++i)
				{
					VBShape* p = new VBShape();
					p->InitialiseShape("WireDiamond2D");
					p->SetScale(0.75);
				
					Vector3 pos = p_pointer->GetPos();
					float a, b, c;
					while (true)
					{
						a = (rand() % ((2 * m_pointsRange) + 1)) - m_pointsRange;
						b = (rand() % ((2 * m_pointsRange) + 1)) - m_pointsRange;
						c = a * a + b * b;
						if (c >= 0 && c <= m_pointsRange * m_pointsRange)
						{
							break;
						}
					}
					pos.x = pos.x + a;
					pos.y = pos.y + b;
					p->SetPos(pos);

					p->NewPartitionObject();
					p->InsertToList();
				}
			}
			p_partitionManager->HighlightPartition();
		}
	}
	if (m_pointsToSpawn > 1)
	{
		m_wireRadius->SetPos(p_pointer->GetPos());
		m_wireRadius->SetScale(m_pointsRange);
		m_wireRadius->Tick(_GD);
	}
}

void UserInterface::Draw(DrawData* _DD)
{
	if (m_pointsToSpawn > 1)
	{
		m_wireRadius->Draw(_DD);
	}
}

void UserInterface::RemoveVariables(list<const char*>* _variables)
{
	for (list<const char*>::iterator it = _variables->begin(); it != _variables->end(); ++it)
	{
		TwRemoveVar(leftUI, (*it));
	}
	_variables->clear();
}

void UserInterface::AddVarCB(TwBar* _bar, const char* _name, TwType _type, TwSetVarCallback _setCallback, TwGetVarCallback _getCallback, 
	void* _clientData, const char* _def, list<const char*>* _list)
{
	TwAddVarCB(_bar, _name, _type, _setCallback, _getCallback, _clientData, _def);
	_list->push_back(_name);
}

void UserInterface::AddVarRW(TwBar* _bar, const char* _name, TwType _type, 
	void* _var, const char* _def, list<const char*>* _list)
{
	TwAddVarRW(_bar, _name, _type, _var, _def);
	_list->push_back(_name);
}

void UserInterface::InterfacePointMethod()
{
	AddVarRW(leftUI, "pointstospawn", TW_TYPE_INT32, &m_pointsToSpawn, " min=1 max=100 label='Points to Place' ", &m_interfaceMethodVariables);
	AddVarRW(leftUI, "pointsrange", TW_TYPE_INT32, &m_pointsRange, " min=0 max=100 label='Points Range' ", &m_interfaceMethodVariables);
}

bool UserInterface::PointWithinBounds(Vector2 _topLeft, Vector2 _bottomRight, Vector2 _point)
{
	if (_point.x >= _topLeft.x)
	{
		if (_point.x <= _bottomRight.x)
		{
			if (_point.y >= _topLeft.y)
			{
				if (_point.y <= _bottomRight.y)
				{
					return true;
				}
			}
		}
	}
	return false;
}