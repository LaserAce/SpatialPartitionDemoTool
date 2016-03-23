//add some light to the scene
#include "light.h"

//a simple light defined by a colour and and ambient colour and used as a point light with no drop off in the standard shader used to draw VBGOs

Light::Light(Vector3 _pos, Color _colour, Color _ambientColour)
{
	m_pos = _pos;
	m_colour = _colour;
	m_ambientColour = _ambientColour;
}

Light::~Light()
{
	m_parent = nullptr;
}

void Light::Tick(GameData* _GD)
{
	_GD;
}