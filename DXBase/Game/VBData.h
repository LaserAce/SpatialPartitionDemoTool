#ifndef _VBDATA_H_
#define _VBDATA_H_

#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct VBData
{
public:
	UINT numPrims;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	D3D_PRIMITIVE_TOPOLOGY topology;
};
#endif