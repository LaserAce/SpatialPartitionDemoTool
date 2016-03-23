//add a light to the scene
#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_
#include "gameobject.h"
#include "pointlightproperties.h"
#include "time.h"

class PointLight : public GameObject
{
public:
	PointLight();
	PointLight(const PointLight &other);
	virtual ~PointLight();

	virtual void Draw(DrawData* _DD){ _DD; }; //draws nothing

	virtual void Tick(GameData* _GD);

	EnhancedLight m_properties;

	bool m_isStrobe = false;
	int m_strobeRate = 1000;
	clock_t m_strobeTimer = 0;
};

#endif