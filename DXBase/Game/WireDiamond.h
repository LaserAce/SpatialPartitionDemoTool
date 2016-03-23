#ifndef _WIRE_DIAMOND_H_
#define _WIRE_DIAMOND_H_

#include "SimpleMath.h"
#include "vertex.h"

struct VBData;

class WireDiamond
{
public:
	static VBData* InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D = true);
};
#endif