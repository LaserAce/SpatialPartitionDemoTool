#ifndef _VB_TEXTURE_CUBE_H_
#define _VB_TEXTURE_CUBE_H_
#include "VBGO.h"
#include "vertex.h"

struct GameData;
struct DrawData;

class TextureCube : public VBGO
{
public:
	TextureCube(){ m_className = "texturecube"; };
	virtual ~TextureCube(){};

	//initialise the Veretx and Index buffers for the cube
	void init(int _size, ID3D11Device* _GD);

	VBData InitialiseBuffer(int _size, ID3D11Device* _GD);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);
	virtual bool Events(Event* event){ return true; };


protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see Spirla, SpikedVB and Pillow
	virtual void Transform(){};

	int m_size;
	myVertex* m_vertices;
};

#endif