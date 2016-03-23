#ifndef _SOLID_CUBE_H_
#define _SOLID_CUBE_H_

#include "VBGO.h"
#include "vertex.h"

struct VBData;

class SolidCube
{
public:
	static VBData* InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D = true);
};
#endif#pragma once
