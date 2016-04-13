//The left hand interface that manages most of the user input
#include "UserInterface.h"
#include "PartitionManager.h"
#include "gamedata.h"
#include <windows.h>
#include "VBShape.h"
#include "Pointer.h"
#include "TestManager.h"
#include "Test.h"
#include "Partition.h"
#include "ScreenMessage.h"

UserInterface* UserInterface::singleton = nullptr;

UserInterface::UserInterface()
{
	singleton = this;
	m_method = INTERFACE_POINT;
	//Initilise the radius that shows the area in which points can be placed around cursor
	m_wireRadius = new VBShape();
	m_wireRadius->InitialiseShape("WireCircle2D");
	m_wireRadius->SetScale(0);

	//Initilise the bold frame that shows where points can be placed
	frameShape = new VBShape();
	frameShape->InitialiseShape("SolidCube2D");
	frameShape->SetScale(202.5f);
	frameShape->SetDefaultColour(Color(0.2, 1, 0));
	frameShape->Tick(nullptr);

	//Initilise a solid fill background for the partitions
	backgroundShape = new VBShape();
	backgroundShape->InitialiseShape("SolidCube2D");
	backgroundShape->SetScale(200);
	backgroundShape->SetDefaultColour(Color(0.0f, 0.0f, 0.0f, 1.0f));
	backgroundShape->Tick(nullptr);
}

UserInterface::~UserInterface()
{
	TwDeleteBar(leftUI);
	delete m_wireRadius;
	if (queryShape)
	{
		delete queryShape;
	}
}

void UserInterface::SetupTwBar()
{
	//Initilise the left bar
	leftUI = TwNewBar("leftUI");
	TwDefine(" leftUI label='Parameter Interface' ");
	TwDefine(" leftUI movable=false ");
	TwDefine(" leftUI resizable=false ");
	TwDefine(" leftUI color='122 122 122' ");
	TwDefine(" leftUI position='0 0' ");
	TwDefine(" leftUI iconifiable=false ");

	AdjustSize();

	//Create all the buttons and sliders for the left bar
	TwEnumVal methods[MAX_METHODS] =
	{
		{ NONE, "Not Active" },
		{ GRID, "Grid" },
		{ QUADTREE, "Quadtree" },
		{ KD_TREE, "KD-tree" },
		{ R_TREE, "R-tree" },
	};
	TwType methodType = TwDefineEnum("MethodType", methods, MAX_METHODS);
	TwAddVarCB(leftUI, "Method", methodType, SetActiveMethod, GetActiveMethod, PartitionManager::Singleton(), " group='Spatial Partition Menu' label='Partitioning Method' ");
	TwAddButton(leftUI, "Rebuild", RebuildPartition, nullptr, " group='Spatial Partition Menu' label='Rebuild Partition' ");
	TwAddVarRW(leftUI, "maxobjects", TW_TYPE_INT32, &m_maxObjects, " group='Spatial Partition Menu' min=0 max=100 label='Max Objects' ");
	TwAddVarRW(leftUI, "maxlevels", TW_TYPE_INT32, &m_maxLevels, "group='Spatial Partition Menu'  min=0 max=100 label='Max Levels' ");
	TwAddButton(leftUI, "Reset", ResetPartition, nullptr, " group='Spatial Partition Menu' label='Reset Partition' ");

	TwAddButton(leftUI, "findtest", FindTest, nullptr, " group='Test Menu' label='Find Test' ");
	TwAddButton(leftUI, "checktest", CheckTest, nullptr, " group='Test Menu' label='Check Test' ");
	TwAddVarRW(leftUI, "buildtest", TW_TYPE_BOOLCPP, TestManager::Singleton()->GetBuildTest(), " group='Test Menu' label='Generate Build Tests' ");

	TwAddVarRW(leftUI, "ViewLevel", TW_TYPE_INT32, PartitionManager::Singleton()->GetViewLevel(), " group='Visuals Menu' min=0 max=100 label='View Level' ");
	TwAddVarRW(leftUI, "DebugVisible", TW_TYPE_BOOLCPP, PartitionManager::Singleton()->GetDebugVisible(), " group='Visuals Menu' label='Partitions Visible' ");
	TwAddVarCB(leftUI, "DebugHighlight", TW_TYPE_BOOLCPP, SetHighlight, GetHighlight, nullptr, " group='Visuals Menu' label='Partitions Highlight' ");

	TwEnumVal interfaceMethods[MAX_INTERFACE_METHODS] =
	{
		{ INTERFACE_POINT, "Point" },
		{ INTERFACE_QUERY, "Query" }
	};
	TwType interfaceMethodType = TwDefineEnum("InterfaceMethodType", interfaceMethods, MAX_INTERFACE_METHODS);
	TwAddVarCB(leftUI, "InterfaceMethod", interfaceMethodType, SetInterfaceMethod, GetInterfaceMethod, this, " group='Mode Menu' label='Mode' ");
	InterfacePointMethod();

	//Desiginate values that represent the inner noin interface section of the screen
	upperLeft = Vector2((float)m_winSize.right * m_size,0.0f);
	lowerRight = Vector2((float)m_winSize.right - ((float)m_winSize.right * m_size), (float)m_winSize.bottom);

	//Set default values to match the current partition defaults
	*UserInterface::Singleton()->GetMaxObjects() = *PartitionManager::Singleton()->GetCurrentRoot()->GetMaxObjects();
	*UserInterface::Singleton()->GetMaxLevels() = *PartitionManager::Singleton()->GetCurrentRoot()->GetMaxLevels();
}

void UserInterface::AdjustSize()
{
	//Adjust size of interface to match window size
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &m_winSize);

	m_size = 0.15;
	int barSize[2] = { (int)((float)m_winSize.right * m_size), m_winSize.bottom };
	TwSetParam(leftUI, NULL, "size", TW_PARAM_INT32, 2, barSize);
}

void TW_CALL UserInterface::RebuildPartition(void* _clientData)
{
	_clientData;
	//Will rebuild the current partition using the anttweakbar values unless is the grid
	if (!(*PartitionManager::Singleton()->GetActiveMethod() == GRID))
	{
		*PartitionManager::Singleton()->GetCurrentRoot()->GetMaxObjects() = *UserInterface::Singleton()->GetMaxObjects();
		*PartitionManager::Singleton()->GetCurrentRoot()->GetMaxLevels() = *UserInterface::Singleton()->GetMaxLevels();
	}
	PartitionManager::Singleton()->RebuildPartition();
}

void TW_CALL UserInterface::ResetPartition(void* _clientData)
{
	_clientData;
	//Resets the anttweakbar to match the default values of the current partition method
	PartitionManager::Singleton()->ResetPartition();
	*UserInterface::Singleton()->GetMaxObjects() = *PartitionManager::Singleton()->GetCurrentRoot()->GetMaxObjects();
	*UserInterface::Singleton()->GetMaxLevels() = *PartitionManager::Singleton()->GetCurrentRoot()->GetMaxLevels();
}

void TW_CALL UserInterface::SpawnPoints(void* _clientData)
{
	//Spawns points randomly
	_clientData;
	for (int i = 0; i < *(int*)_clientData; ++i)
	{
		VBShape* _s = new VBShape();
		_s->InitialiseShape("WireDiamond2D");
		_s->SetScale(0.75);
		int _randx = (rand() % 401) - 200;
		int _randy = (rand() % 401) - 200;
		_s->SetPos(Vector3((float)_randx, (float)_randy, 0.0f));
		_s->NewPartitionObject();
		_s->InsertToList();
		_s->SetDefaultColour(Color(0.1f, 0.5f, 0));
	}
}


void TW_CALL UserInterface::DeletePoints(void* _clientData)
{
	//Deletes all placed points
	_clientData;
	PartitionManager::Singleton()->DeletePoints();
}

void TW_CALL UserInterface::FindTest(void* _clientData)
{
	_clientData;
	//Using the current drawn query, find all points within
	VBShape* _shape = UserInterface::Singleton()->GetQueryBox();
	if (_shape)
	{
		float _hori = _shape->GetScale().x;
		float _vert = _shape->GetScale().y;
		Vector3 _pos = _shape->GetPos();
		TestManager::Singleton()->GenerateFindTest(Vector2(_pos.x - _hori, _pos.y + _vert), Vector2(_pos.x + _hori, _pos.y - _vert));
	}
	else
	{
		ScreenMessage::Singleton()->DisplayMessage(L"A query rect needs to be drawn to perform this test");
	}
}

void TW_CALL UserInterface::CheckTest(void* _clientData)
{
	_clientData;
	//Using the current drawn query find the amount of checks that will happen
	VBShape* _shape = UserInterface::Singleton()->GetQueryBox();
	if (_shape)
	{
		float _hori = _shape->GetScale().x;
		float _vert = _shape->GetScale().y;
		Vector3 _pos = _shape->GetPos();
		TestManager::Singleton()->GenerateCheckTest(Vector2(_pos.x - _hori, _pos.y + _vert), Vector2(_pos.x + _hori, _pos.y - _vert));
	}
	else
	{
		ScreenMessage::Singleton()->DisplayMessage(L"A query rect needs to be drawn to perform this test");
	}
}

void TW_CALL UserInterface::SetActiveMethod(const void *value, void *clientData)
{
	//Sets the current active partition method
	PartitionManager* man = static_cast<PartitionManager*>(clientData);

	man->SetActiveMethod(*static_cast<const PartitionMethods*>(value));

	*UserInterface::Singleton()->GetMaxObjects() = *man->GetCurrentRoot()->GetMaxObjects();
	*UserInterface::Singleton()->GetMaxLevels() = *man->GetCurrentRoot()->GetMaxLevels();
}

void  TW_CALL UserInterface::GetActiveMethod(void *value, void *clientData)
{
	PartitionManager* man = static_cast<PartitionManager*>(clientData);
	*(PartitionMethods*)value = *man->GetActiveMethod();
}

void TW_CALL UserInterface::SetInterfaceMethod(const void *value, void *clientData)
{
	//Sets the current active UI element - Point or Query
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

void  TW_CALL UserInterface::GetInterfaceMethod(void* _value, void* _clientData)
{
	UserInterface* ui = static_cast<UserInterface*>(_clientData);
	*(InterfaceMethod*)_value = *ui->GetInterfaceMehod();
}

void TW_CALL UserInterface::SetHighlight(const void *_value, void *_clientData)
{
	//Set whether partition boxes should highlight
	_clientData;
	PartitionManager::Singleton()->UnHighlightPartition();
	*PartitionManager::Singleton()->GetHighlight() = *(bool*)_value;
}

void  TW_CALL UserInterface::GetHighlight(void* _value, void* _clientData)
{
	_clientData;
	*(bool*)_value = *PartitionManager::Singleton()->GetHighlight();
}

void UserInterface::Tick(GameData* _GD)
{
	//Tick on certain mouse events
	if (_GD->mouse->rgbButtons[0] && !_GD->prevMouse->rgbButtons[0])
	{
		MouseClick();
	}
	else if (_GD->mouse->rgbButtons[0] && _GD->prevMouse->rgbButtons[0])
	{
		MouseHold();
	}
	else if (!_GD->mouse->rgbButtons[0] && _GD->prevMouse->rgbButtons[0])
	{
		MouseRelease();
	}
	//Only update wire radius if placing more than 1 point
	if (m_pointsToSpawn > 1)
	{
		m_wireRadius->SetPos(Pointer::Singleton()->GetPos());
		m_wireRadius->SetScale((float)m_pointsRange);
		m_wireRadius->Tick(_GD);
	}
	else
	{
		m_wireRadius->SetScale(0);
		m_wireRadius->Tick(_GD);
	}
	//Only update querybox if one exists
	if (queryShape)
	{
		queryShape->Tick(_GD);
	}
}

void UserInterface::MouseClick()
{
	//Get mouse cursor location
	POINT cursor;
	GetCursorPos(&(cursor));
	const HWND hDesktop = GetDesktopWindow();
	ScreenToClient(hDesktop, &cursor);

	//If not over the interface
	if (PointWithinBounds(upperLeft, lowerRight, Vector2((float)cursor.x, (float)cursor.y)))
	{
		switch (m_method)
		{
		case INTERFACE_POINT:
			//Create a single point precisily 
			PartitionManager::Singleton()->UnHighlightPartition();
			if (m_pointsToSpawn == 1)
			{
				//If within the inner section of the screen
				if (PointWithinBounds(Vector2(-200,200), Vector2(200, -200), Pointer::Singleton()->GetPos()))
				{
					VBShape* p = new VBShape();
					p->InitialiseShape("WireDiamond2D");
					p->SetScale(0.75);
					p->SetPos(Pointer::Singleton()->GetPos());
					p->NewPartitionObject();
					p->InsertToList();
					p->SetDefaultColour(Color(0.1f, 0.5f, 0));
				}
			}
			//Create multiple points randomly
			else
			{
				for (int i = 0; i < m_pointsToSpawn; ++i)
				{
					VBShape* p = new VBShape();
					
					//Generate random points within circle radius
					Vector3 pos = Pointer::Singleton()->GetPos();
					float a, b, c;
					while (true)
					{
						a = (float)(rand() % ((2 * m_pointsRange) + 1)) - m_pointsRange;
						b = (float)(rand() % ((2 * m_pointsRange) + 1)) - m_pointsRange;
						c = a * a + b * b;
						if (c >= 0 && c <= m_pointsRange * m_pointsRange)
						{
							break;
						}
					}

					pos.x = pos.x + a;
					pos.y = pos.y + b;

					//If within the inner section of the screen
					if (PointWithinBounds(Vector2(-200, 200), Vector2(200, -200), pos))
					{
						p->InitialiseShape("WireDiamond2D");
						p->SetScale(0.75);
						p->SetPos(pos);
						p->NewPartitionObject();
						p->InsertToList();
						p->SetDefaultColour(Color(0.1f, 0.5f, 0));
					}
					else
					{
						//Point has been spawned outside safe area
						delete p;
					}
				}
			}
			PartitionManager::Singleton()->HighlightPartition();
			break;
		case INTERFACE_QUERY:
			//If not already drawing query then start drawing query
			if (!isDrawingQuery)
			{
				isDrawingQuery = true;
				queryPivot = Pointer::Singleton()->GetPos();
				//If query doesn't exist yet then create one
				if (!queryShape)
				{
					queryShape = new VBShape();
					queryShape->InitialiseShape("SolidCube2D");
					queryShape->SetColour(Color(1.0f, 0.0f, 0.0f, 0.25f));
				}
			}
			break;
		}
	}
}

void UserInterface::MouseHold()
{
	//Get mouse cursor location
	POINT cursor;
	GetCursorPos(&(cursor));
	const HWND hDesktop = GetDesktopWindow();
	ScreenToClient(hDesktop, &cursor);

	switch (m_method)
	{
	case INTERFACE_QUERY:
		//If not on the interface
		if (PointWithinBounds(upperLeft, lowerRight, Vector2((float)cursor.x, (float)cursor.y)))
		{
			//if drawing query then update position and scale 
			if (isDrawingQuery)
			{
				PositionQuery();
			}
		}
		else
		{
			//If drawing query and moved onto interface then finish query there
			if (isDrawingQuery)
			{
				CreateQuery();
			}
		}
		break;
	}
}

void UserInterface::MouseRelease()
{
	//Get mouse cursor location
	POINT cursor;
	GetCursorPos(&(cursor));
	const HWND hDesktop = GetDesktopWindow();
	ScreenToClient(hDesktop, &cursor);

	switch (m_method)
	{
	case INTERFACE_QUERY:
		//Wherever the query is being drawn stop and finish query
		if (isDrawingQuery)
		{
			CreateQuery();
		}
		break;
	}
}

void UserInterface::CreateQuery()
{
	//Stop drawing query
	isDrawingQuery = false;
	//If multiple queries then add to a list
}

void UserInterface::PositionQuery()
{
	//Adjust position and scale of query to keep it between starting position and cursor
	Vector3 _pointer = Pointer::Singleton()->GetPos();
	queryShape->SetPos((queryPivot + _pointer) / 2);
	queryShape->SetScale(Vector3(abs(queryPivot.x - _pointer.x)/2, abs(queryPivot.y - _pointer.y)/2,0.0f));
}

void UserInterface::Draw(DrawData* _DD)
{
	//Draw backgrounds first
	frameShape->Draw(_DD);
	backgroundShape->Draw(_DD);

	//Draw radius only when method is point and placing more than 1
	if (m_method == INTERFACE_POINT)
	{
		if (m_pointsToSpawn > 1)
		{
			m_wireRadius->Draw(_DD);
		}
	}
	//Draw query box if it exists
	if (queryShape)
	{
		queryShape->Draw(_DD);
	}
}

void UserInterface::RemoveVariables(list<const char*>* _variables)
{
	//Removes all variables within list from anttweakbar
	for (list<const char*>::iterator it = _variables->begin(); it != _variables->end(); ++it)
	{
		TwRemoveVar(leftUI, (*it));
	}
	_variables->clear();
}

//A series of custom functions that manage antweakbar variables
//to make create and deletoin easier and less hard coded
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

void UserInterface::AddButton(TwBar* _bar, const char* _name, TwButtonCallback _callback, 
	void* _clientData, const char* _def, list<const char*>* _list)
{
	TwAddButton(_bar, _name, _callback, _clientData, _def);
	_list->push_back(_name);
}

void UserInterface::InterfacePointMethod()
{
	//The antweakbar variables that only show when the interface option is Point
	AddVarRW(leftUI, "pointstospawn", TW_TYPE_INT32, &m_pointsToSpawn, " group = 'Mode Menu' min=1 max=5000 label='Points to Place' ", &m_interfaceMethodVariables);
	AddVarRW(leftUI, "pointsrange", TW_TYPE_INT32, &m_pointsRange, " group = 'Mode Menu' min=0 max=250 label='Points Range' ", &m_interfaceMethodVariables);
	AddButton(leftUI, "spawnpoints", SpawnPoints, &m_pointsToSpawn, " group = 'Mode Menu' label='Spawn Random Points' ", &m_interfaceMethodVariables);
	AddButton(leftUI, "deletepoints", DeletePoints, nullptr, " group = 'Mode Menu' label='Delete All Points' ", &m_interfaceMethodVariables);
	
}

bool UserInterface::PointWithinBounds(Vector2 _topLeft, Vector2 _bottomRight, Vector2 _point)
{
	//Test the location of the cursor within screen space
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

bool UserInterface::PointWithinBounds(Vector2 _topLeft, Vector2 _bottomRight, Vector3 _point)
{
	//Test the location of the cursor within world space
	if (_point.x >= _topLeft.x)
	{
		if (_point.x <= _bottomRight.x)
		{
			if (_point.y <= _topLeft.y)
			{
				if (_point.y >= _bottomRight.y)
				{
					return true;
				}
			}
		}
	}
	return false;
}