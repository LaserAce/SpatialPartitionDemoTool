#ifndef _VB_SHAPE_H_
#define _VB_SHAPE_H_

#include "VBGO.h"
#include "vertex.h"
#include <time.h>
#include <map>
#include <string>

using std::map;
using std::pair;
using std::string;

struct GameData;
struct DrawData;
struct VBData;

//Used to store buffer information with identification string
typedef map<string, VBData*> VBMap;

class VBShape : public VBGO
{
public:
	//Constructor
	VBShape();
	//Deconstructor
	virtual ~VBShape();

	//Initialise entity
	void InitialiseShape(string shapeName);
	static void AddShapes(ID3D11Device* GD);

	//Tick
	virtual void Tick(GameData* _GD);
	//Draw
	virtual void Draw(DrawData* _DD);


	static VBMap s_VBHolder;

	void SetColour(Color _c){ m_currentColour = _c; }
	Color GetColour(){ return m_currentColour; }
	void SetDefaultColour(Color _c) { m_defaultColour = _c; SetToDefaultColour(); }
	void SetToDefaultColour(){ m_currentColour = m_defaultColour; }

protected:
	myVertex* m_vertices;   //Contains all the vertices of this entity
	Color m_currentColour;
	Color m_defaultColour;




	
};

#endif