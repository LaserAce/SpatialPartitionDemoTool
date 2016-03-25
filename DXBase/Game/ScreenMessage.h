//This class manages showing text to the screen
#ifndef _SCREEN_MESSAGE_H_
#define _SCREEN_MESSAGE_H_

#include "GameObject2D.h"

class ScreenMessage : public GameObject2D
{
public:
	ScreenMessage();
	virtual ~ScreenMessage();

	static ScreenMessage* Singleton() { return singleton; }

	virtual void tick(GameData* _GD);
	virtual void draw(DrawData2D* _DD);

	void DisplayMessage(const WCHAR* _message);

protected:
	
	static ScreenMessage* singleton;

	RECT m_winSize;
	const WCHAR* m_message;
	int timeAtDisplay = 0;
	int displayDuration = 2000;
	Vector2 dimensions;
	bool dimensionsDoOnce = false;
	Vector2 titleDimensions;
};


#endif