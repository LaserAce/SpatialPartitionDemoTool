#ifndef _POINTER_H_
#define _POINTER_H_

#include "VBShape.h"

struct GameData;
struct DrawData;

class Camera;

class Pointer : public VBShape
{
public:
	Pointer();
	virtual ~Pointer();

	static Pointer* Singleton() { return singleton; }

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	void SetCamera(Camera* _cam) { p_cam = _cam; }

protected:
	static Pointer* singleton;
	void Trace();
	Camera* p_cam;
	
};

#endif