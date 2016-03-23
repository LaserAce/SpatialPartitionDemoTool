//Vertex Structure
#ifndef _POINT_LIGHT_PROPERTIESH_H_
#define _POINT_LIGHT_PROPERTIESH_H_
#include <d3d11_1.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

#ifndef MAX_LIGHTS 
#define MAX_LIGHTS 8
#endif
__declspec(align(16))
struct EnhancedLight
{
	Vector4 pos = Vector4(0.0f,0.0f,0.0f,1.0f);
	Color color = Color(1.0f,1.0f,1.0f,1.0f);
	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.08f;
	float quadraticAttenuation = 0.0f;
	int enabled = false;
};
__declspec(align(16))
struct LightProperties
{
	//static Vector4 cameraPosition;
	//static Color globalAmbient;
	Vector4 cameraPosition;
	Color globalAmbient;
	EnhancedLight lights[MAX_LIGHTS];
};


#endif