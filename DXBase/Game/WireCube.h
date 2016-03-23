#ifndef _WIRE_CUBE_H_
#define _WIRE_CUBE_H_
#include "VBGO.h"
#include "vertex.h"
//procedurally generate a VBGO Cube
//each side be divided in to _size * _size squares (2 triangles per square)

struct GameData;
struct DrawData;

class WireCube : public VBGO
{
public:
	WireCube(){};
	virtual ~WireCube(){};

	VBData InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D = true);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see Spirla, SpikedVB and Pillow
	virtual void Transform(){};

	
};

#endif