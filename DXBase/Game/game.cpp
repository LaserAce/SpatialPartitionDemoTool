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
#include "light.h"
#include "Pointer.h"
#include "PartitionObject.h"
#include "PartitionManager.h"
#include "UserInterface.h"
#include "TestManager.h"
#include "ScreenMessage.h"



using namespace DirectX;

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
	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// initialize the mouse
	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);


	m_GD = new GameData();
	m_GD->keyboard = m_keyboardState;
	m_GD->prevKeyboard = m_prevKeyboardState;
	m_GD->mouse = &m_mouse_state;
	m_GD->prevMouse = &m_prev_mouse_state;


	m_Cam = new Camera(0.25f * XM_PI, 640.0f, 480.0f, 1.0f, 10000.0f, Vector3::Zero, Vector3::UnitY);
	m_Cam->SetPos( Vector3(0.0f, 0.0f, 100.0f));
	
	m_GameObjects.push_back(m_Cam);
	

	m_DD = new DrawData();
	m_DD->light = new Light(Vector3(0,100,0),Color(0,0,0), Color(0,0,0));


	
	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);
	
	// Create DirectXTK spritebatch stuff
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"italic.spritefont"));
	
	//create Draw Data
	m_DD->pd3dImmediateContext = pd3dImmediateContext;
	m_DD->states = m_States;
	
	//initilise the defaults for the VBGOs
	VBGO::Init(_pd3dDevice);

	VBShape::AddShapes(_pd3dDevice);

	m_UI = new UserInterface();
	m_GameObjects.push_back(m_UI);

	PartitionManager* partitionManager = new PartitionManager();
	m_GameObjects.push_back(partitionManager);
	PartitionObject::s_partitionManager = partitionManager;

	Pointer* p = new Pointer();
	p->SetCamera(m_Cam);
	p->InitialiseShape("WireDiamond3D");
	p->SetScale(5);
	m_GameObjects.push_back(p);

	m_TM = new TestManager();
	m_GameObjects.push_back(m_TM);

	m_GameObject2Ds.push_back(new ScreenMessage());

	for (int i = 0; i < 500; ++i)
	{
		VBShape* s = new VBShape();
		s->InitialiseShape("WireDiamond2D");
		s->SetScale(0.75);
		int randx = (rand() % 401) - 200;
		int randy = (rand() % 401) - 200;
		s->SetPos(Vector3((float)randx, (float)randy, 0.0f));
		s->NewPartitionObject();
		s->InsertToList();
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
}

void Game::InitAntTweak()
{
	m_UI->SetupTwBar();
	m_TM->SetupTwBar();
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
	DWORD currentTime = GetTickCount();
	m_GD->dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;


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
