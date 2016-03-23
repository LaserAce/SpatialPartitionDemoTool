//Vertex Structure
#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include <d3d11_1.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

__declspec(align(16))
struct MyMaterial
{
	Color ambient = Color(0.1f,0.1f,0.1f,1.0f);
	Color diffuse = Color(1.0f,1.0f,1.0f,1.0f);
	Color emissive = Color(0.0f,0.0f,0.0f,1.0f);
	Color specular = Color(1.0f,1.0f,1.0f,1.0f);
	float shininess = 128.0f;
	float padding[3];
};

__declspec(align(16))
struct MaterialProperties
{
	MyMaterial material;
};

#endif