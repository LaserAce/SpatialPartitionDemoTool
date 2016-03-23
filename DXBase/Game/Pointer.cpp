#include "Pointer.h"
#include "camera.h"

Pointer::Pointer(Camera* _cam)
{
	p_cam = _cam;
	m_defaultColour = Color(2, 0.2, 0.2);
	m_currentColour = m_defaultColour;
}

Pointer::~Pointer()
{

}

void Pointer::Tick(GameData* _GD)
{
	Trace();
	VBShape::Tick(_GD);
}

void Pointer::Draw(DrawData* _DD)
{
	VBShape::Draw(_DD);
}

void Pointer::Trace()
{
	RECT rc;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &rc);

	POINT cursor;
	GetCursorPos(&(cursor));
	ScreenToClient(hDesktop, &cursor);

	//From 2D Space to 3D space
	float pointX = ((2.0f * (float)cursor.x) / (float)rc.right) - 1.0f;
	float pointY = (((-2.0f * (float)cursor.y) / (float)rc.bottom) + 1.0f);
	Matrix m = p_cam->GetProj() * p_cam->GetView();
	m.Invert();
	Vector3 pos = Vector3(pointX, pointY, 0);
	Vector3::Transform(pos, m);

	//Alter position of object to match cursor
	m_pos.x = (pos.x * (p_cam->GetWidth() / 2)) + p_cam->GetPos().x; 
	m_pos.y = (pos.y * ((p_cam->GetWidth()/p_cam->GetAspectRatio()) / 2)) + p_cam->GetPos().y;
}
