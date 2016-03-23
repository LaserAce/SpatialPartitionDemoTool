#ifndef _POINTER_H_
#define _POINTER_H_

#include "VBShape.h"

struct GameData;
struct DrawData;

class Camera;

class Pointer : public VBShape
{
public:
	//Constructor
	Pointer(Camera* _cam);
	//Deconstructor
	virtual ~Pointer();

	//Tick
	virtual void Tick(GameData* _GD);
	//Draw
	virtual void Draw(DrawData* _DD);

protected:
	void Trace();
	Camera* p_cam;
	
};

#endif