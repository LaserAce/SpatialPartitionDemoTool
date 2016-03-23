//add some light to the scene
#include "pointlight.h"
#include "gamedata.h"
#include "GameCamera.h"
#include "game.h"
#include "locator.h"
//a simple light defined by a colour and and ambient colour and used as a point light with no drop off in the standard shader used to draw VBGOs

PointLight::PointLight()
{
	m_className = "pointlight";
	Locator::GetGame()->GetPointLights()->push_back(this);
	m_strobeTimer = clock();// +(rand() % 200 + 1);
}

//Copy constructor
PointLight::PointLight(const PointLight &_other) : GameObject(_other)
{
	m_className = "pointlight";
	Locator::GetGame()->GetPointLights()->push_back(this);
	m_properties.color = _other.m_properties.color;
	m_properties.constantAttenuation = _other.m_properties.constantAttenuation;
	m_properties.linearAttenuation = _other.m_properties.linearAttenuation;
	m_properties.quadraticAttenuation = _other.m_properties.quadraticAttenuation;
	m_properties.enabled = _other.m_properties.enabled;
	m_isStrobe = _other.m_isStrobe;
	m_strobeRate = _other.m_strobeRate;
}

PointLight::~PointLight()
{
	list<PointLight*>* lights = Locator::GetGame()->GetPointLights();
	for (list<PointLight*>::iterator it = lights->begin(); it != lights->end(); ++it)
	{
		if ((*it) == this)
		{
			lights->erase(it);
			return;
		}
	}
}

void PointLight::Tick(GameData* _GD)
{
	//if (_GD->GS == GS_PLAY_GAME)
	//{
		if (m_isStrobe)
		{
			clock_t diff = clock() - m_strobeTimer;
			if (diff > m_strobeRate && m_properties.enabled)
			{
				m_properties.enabled = false;
				m_strobeTimer = clock();
			}
			else if (diff > m_strobeRate && !m_properties.enabled)
			{
				m_strobeTimer = 0;
				m_properties.enabled = true;
				m_strobeTimer = clock();
			}
		}
	//}
	m_properties.pos = (Vector4)m_pos;
	GameObject::Tick(_GD);
}