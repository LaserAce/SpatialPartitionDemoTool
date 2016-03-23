#ifndef _WIRE_CUBE_H_
#define _WIRE_CUBE_H_

#include "VBGO.h"
#include "vertex.h"

struct VBData;

class WireCube
{
public:
	static VBData* InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D = true);
};
#endif