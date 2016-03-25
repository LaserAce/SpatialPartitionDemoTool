//This class manages showing text to the screen
#include "ScreenMessage.h"
#include "DrawData2D.h"

using namespace DirectX;

ScreenMessage* ScreenMessage::singleton = nullptr;

ScreenMessage::ScreenMessage()
{
	singleton = this;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &m_winSize);
}

ScreenMessage::~ScreenMessage()
{
	
}

void ScreenMessage::tick(GameData* _GD)
{
	_GD;
}

void ScreenMessage::draw(DrawData2D* _DD)
{
	//Setup the title if it has not been setup
	if (titleDimensions == Vector2::Zero)
	{
		titleDimensions = _DD->m_Font->MeasureString(L"Demonstration Tool for Spatial Partition Systems");
	}
	//Draw title
	_DD->m_Font->DrawString(_DD->m_Sprites.get(), L"Demonstration Tool for Spatial Partition Systems", XMFLOAT2((float)m_winSize.right / 2,  (float)m_winSize.bottom * 0.04f),
		Colors::DarkGreen, 0, XMFLOAT2(titleDimensions.x / 2, titleDimensions.y / 2), 1.0f, SpriteEffects_None, 0.5f);
	//If a message should be shown
	if (timeAtDisplay + displayDuration > (int)GetTickCount64())
	{
		//Setup the message once
		if (dimensionsDoOnce)
		{
			dimensions = _DD->m_Font->MeasureString(m_message);
			dimensionsDoOnce = false;
		}
		//Draw message
		_DD->m_Font->DrawString(_DD->m_Sprites.get(), m_message, XMFLOAT2((float)m_winSize.right/2, (float)m_winSize.bottom * 0.96f), 
			Colors::DarkRed, 0, XMFLOAT2(dimensions.x / 2, dimensions.y / 2), 1.0f, SpriteEffects_None, 0.5f);
	}
}

void ScreenMessage::DisplayMessage(const WCHAR* _message)
{
	//Sets up a message on a timer
	m_message = _message;
	timeAtDisplay = (int)GetTickCount64();
	dimensionsDoOnce = true;
}
