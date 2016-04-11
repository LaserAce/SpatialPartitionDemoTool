//Main game/application class
#include "game.h"
#include "gameobject.h"
#include "GameObject2D.h"
#include <windows.h>
#include "CommonStates.h"
#include "gamedata.h"
#include <time.h>
#include "helper.h"
#include "DDSTextureLoader.h"
#include "drawdata.h"
#include "DrawData2D.h"
#include "camera.h"
#include "VBShape.h"
#include "Pointer.h"
#include "PartitionObject.h"
#include "PartitionManager.h"
#include "UserInterface.h"
#include "TestManager.h"
#include "ScreenMessage.h"

extern HWND g_hWnd;

Game::Game(ID3D11Device* _pd3dDevice, HINSTANCE _hInstance) :m_playTime(0), m_myEF(nullptr)
{
	GameObject::p_gameObjects = &m_GameObjects;
	/* initialize random seed: */
	srand(static_cast<unsigned int>(time(NULL)));

	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	//set up DirectXTK Effects system
	m_myEF  = new MyEffectFactory(_pd3dDevice);
#if DEBUG
	m_myEF->SetPath(L"Debug\\");
#else
	m_myEF->SetPath(L"Release\\");
#endif

	
	// Create other render resources here
	m_States=new DirectX::CommonStates(_pd3dDevice);

	//Direct Input Stuff
	HRESULT _hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	_hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	_hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	_hr = m_pKeyboard->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// initialize the mouse
	_hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	_hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	_hr = m_pMouse->SetDataFormat(&c_dfDIMouse);

	//Setup the gamedata that will passed around in tick
	m_GD = new GameData();
	m_GD->keyboard = m_keyboardState;
	m_GD->prevKeyboard = m_prevKeyboardState;
	m_GD->mouse = &m_mouse_state;
	m_GD->prevMouse = &m_prev_mouse_state;

	//Setup the starting values for the camera
	m_Cam = new Camera(0.25f * XM_PI, 640.0f, 480.0f, 1.0f, 10000.0f, Vector3::Zero, Vector3::UnitY);
	m_Cam->SetPos( Vector3(0.0f, 0.0f, 100.0f));
	m_GameObjects.push_back(m_Cam);
	
	


	ID3D11DeviceContext* _pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&_pd3dImmediateContext);
	
	//Setup the drawdata that will passed around in gameobject tick
	m_DD = new DrawData();
	m_DD->pd3dImmediateContext = _pd3dImmediateContext;
	m_DD->states = m_States;

	//Setup the drawdata2d that will be passed around in the gameobject2d tick
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(_pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"italic.spritefont"));
	
	//initilise the defaults for the VBGOs and VBShapes
	VBGO::Init(_pd3dDevice);
	VBShape::AddShapes(_pd3dDevice);

	//Create the interfaces above everything else
	m_GameObjects.push_back(new UserInterface());
	m_GameObjects.push_back(new TestManager());

	//Create the partitions next
	m_GameObjects.push_back(new PartitionManager());
	
	//Create the pointer that will follow the mouse cursor
	Pointer* _p = new Pointer();
	_p->SetCamera(m_Cam);
	_p->InitialiseShape("WireDiamond3D");
	_p->SetScale(5);
	m_GameObjects.push_back(_p);

	//Create a display message that can show text to the screen
	m_GameObject2Ds.push_back(new ScreenMessage());

	//Randomly generate some starting points
	for (int i = 0; i < 2000; ++i)
	{
		VBShape* _s = new VBShape();
		_s->InitialiseShape("WireDiamond2D");
		_s->SetScale(0.75);
		int _randx = (rand() % 401) - 200;
		int _randy = (rand() % 401) - 200;
		_s->SetPos(Vector3((float)_randx, (float)_randy, 0.0f));
		_s->NewPartitionObject();
		_s->InsertToList();
	}
}

Game::~Game()
{
	//VBGO::CleanUp();

	//tidy away Direct Input Stuff
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pDirectInput) m_pDirectInput->Release();

	
	//get rid of the game objects here
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		delete (*it);
	}

	m_GameObjects.clear();
	//and the 2D ones
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		delete (*it);
	}
	m_GameObject2Ds.clear();

	delete m_States;
	delete m_myEF;
	delete m_GD;
	delete m_DD;
	delete m_DD2D;
}

bool Game::update()
{
	ReadKeyboard();
	ReadMouse();

	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return false;
	}
	
	//calculate frame time-step dt for passing down to game objects
	DWORD _currentTime = GetTickCount();
	m_GD->dt = min((float)(_currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = _currentTime;

	//Tick/update all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(m_GD);
	}
	
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->tick(m_GD);
	}

	return true;
}

void Game::render(ID3D11DeviceContext* _pd3dImmediateContext)
{
	
	m_DD->pd3dImmediateContext = _pd3dImmediateContext;
	m_DD->cam = m_Cam;

	//VBGO::UpdateConstantBuffer(m_DD);

	//draw all 3D objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Draw(m_DD);
	}
	
	// Draw sprite batch stuff
	m_DD2D->m_Sprites->Begin();

	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{

		(*it)->draw(m_DD2D);
	}

	m_DD2D->m_Sprites->End();

	TwDraw();

	_pd3dImmediateContext->OMSetDepthStencilState(m_States->DepthDefault(), 0);
}

void Game::InitAntTweak()
{
	//Initilise anttweakbars
	UserInterface::Singleton()->SetupTwBar();
	TestManager::Singleton()->SetupTwBar();
}

bool Game::ReadKeyboard()
{
	//copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(m_keyboardState));

	//clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(m_keyboardState));

	// Read the keyboard device.
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;

}

bool Game::ReadMouse()
{
	memcpy(&m_prev_mouse_state, &m_mouse_state, sizeof(m_mouse_state));
	//clear out previous state
	ZeroMemory(&m_mouse_state, sizeof(m_mouse_state));

	// Read the Mouse device.
	HRESULT hr = m_pMouse->GetDeviceState(sizeof(m_mouse_state), (LPVOID)&m_mouse_state);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;

}
