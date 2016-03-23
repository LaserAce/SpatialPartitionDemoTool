//base camera class
#include "camera.h"
#include "gamedata.h"

Camera::Camera(float _fieldOfView, float _width, float _height, float _nearPlaneDistance, float _farPlaneDistance, Vector3 _target, Vector3 _up)
{
	m_fieldOfView = _fieldOfView;
	m_width = _width;
	m_height = _height;
	m_aspectRatio = _width/ _height;
	m_nearPlaneDistance = _nearPlaneDistance;
	m_farPlaneDistance = _farPlaneDistance;

	m_target = _target;
	m_up = _up;
}

Camera::~Camera()
{
}

void Camera::Tick(GameData* _GD)
{
	RECT rc;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &rc);

	float camSpeed = 10.0f;
	if (_GD->keyboard[DIK_W] & 0x80)
	{
		m_pos.y += _GD->dt * camSpeed;
		m_target.y += _GD->dt * camSpeed;
	}
	if (_GD->keyboard[DIK_S] & 0x80)
	{
		m_pos.y -= _GD->dt * camSpeed;
		m_target.y -= _GD->dt * camSpeed;
	}
	if (_GD->keyboard[DIK_A] & 0x80)
	{
		m_pos.x -= _GD->dt * camSpeed;
		m_target.x -= _GD->dt * camSpeed;
	}
	if (_GD->keyboard[DIK_D] & 0x80)
	{
		m_pos.x += _GD->dt * camSpeed;
		m_target.x += _GD->dt * camSpeed;
	}
	float zoomSpeed = 50.0f;
	if (_GD->keyboard[DIK_Q] & 0x80)
	{
		m_width -= _GD->dt * zoomSpeed;
	}
	if (_GD->keyboard[DIK_E] & 0x80)
	{
		m_width += _GD->dt * zoomSpeed;
	}

	//m_projMat = Matrix::CreatePerspectiveFieldOfView(m_fieldOfView, m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);
	m_projMat = Matrix::CreateOrthographic(m_width, m_width/m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);
	m_viewMat = Matrix::CreateLookAt(m_pos, m_target, m_up);

	GameObject::Tick(_GD);
}

void Camera::Draw(DrawData* _DD)
{
	_DD;
	//standard camera doesn't draw ANYTHING
}