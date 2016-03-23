#ifndef _WIRE_CIRCLE_H_
#define _WIRE_CIRCLE_H_

#include "vertex.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct VBData;

class WireCircle
{
public:
	static VBData* InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D = true);
};

#endif