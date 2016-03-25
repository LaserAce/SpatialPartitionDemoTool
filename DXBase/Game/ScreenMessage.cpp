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

void ScreenMessage::DisplayMessage(const WCHAR* _message)
{
	m_message = _message;
	timeAtDisplay = (int)GetTickCount64();
	dimensionsDoOnce = true;
}

void ScreenMessage::tick(GameData* _GD)
{
	_GD;
}

void ScreenMessage::draw(DrawData2D* _DD)
{
	if (titleDimensions == Vector2::Zero)
	{
		titleDimensions = _DD->m_Font->MeasureString(L"Demonstration Tool for Spatial Partition Systems");
	}
	_DD->m_Font->DrawString(_DD->m_Sprites.get(), L"Demonstration Tool for Spatial Partition Systems", XMFLOAT2((float)m_winSize.right / 2,  (float)m_winSize.bottom * 0.04f),
		Colors::DarkGreen, 0, XMFLOAT2(titleDimensions.x / 2, titleDimensions.y / 2), 1.0f, SpriteEffects_None, 0.5f);
	if (timeAtDisplay + displayDuration > (int)GetTickCount64())
	{
		if (dimensionsDoOnce)
		{
			dimensions = _DD->m_Font->MeasureString(m_message);
			dimensionsDoOnce = false;
		}
		_DD->m_Font->DrawString(_DD->m_Sprites.get(), m_message, XMFLOAT2((float)m_winSize.right/2, (float)m_winSize.bottom * 0.96f), 
			Colors::DarkRed, 0, XMFLOAT2(dimensions.x / 2, dimensions.y / 2), 1.0f, SpriteEffects_None, 0.5f);
	}
}