#include "VBShape.h"
#include "WireCube.h"
#include "WireDiamond.h"
#include "WireCircle.h"
#include "drawdata.h"
#include "gamedata.h"
#include "camera.h"
#include "light.h"

VBMap VBShape::s_VBHolder;

VBShape::VBShape()
{
	//m_colour = Color(20, 120, 0);
	m_defaultColour = Color(0.2,1,0);
	m_currentColour = m_defaultColour;
}

VBShape::~VBShape()
{
	
}

void VBShape::AddShapes(ID3D11Device* _GD)
{
	string name;
	VBData data;

	WireCube* cube = new WireCube();
	data = cube->InitialiseBuffer(_GD, true);
	s_VBHolder.insert(make_pair("WireCube3D", data));

	cube = new WireCube();
	data = cube->InitialiseBuffer(_GD, false);
	s_VBHolder.insert(make_pair("WireCube2D", data));

	WireDiamond* diamond = new WireDiamond();
	data = diamond->InitialiseBuffer(_GD, true);
	s_VBHolder.insert(make_pair("WireDiamond3D", data));

	diamond = new WireDiamond();
	data = diamond->InitialiseBuffer(_GD, false);
	s_VBHolder.insert(make_pair("WireDiamond2D", data));

	WireCircle* circle = new WireCircle();
	data = circle->InitialiseBuffer(_GD, false);
	s_VBHolder.insert(make_pair("WireCircle2D", data));

	delete cube;
	delete diamond;
	delete circle;
}

void VBShape::InitialiseShape(string _shapeName)
{
	m_pCB = new ConstantBuffer();
	ZeroMemory(m_pCB, sizeof(ConstantBuffer));
	VBMap::iterator it = s_VBHolder.find(_shapeName);
	if (it != s_VBHolder.end())
	{
		m_numPrims = (*it).second.first;
		m_VertexBuffer = (*it).second.second[0];
		m_IndexBuffer = (*it).second.second[1];
	}
	else
	{
		//Create new shape or break horribly or something
	}
}

void VBShape::Tick(GameData* _GD)
{
	VBGO::Tick(_GD);
}
void VBShape::Draw(DrawData* _DD)
{
	//Here we update the constant buffer and feed in the color of our object "currColor"
	((ConstantBuffer*)m_pCB)->view = _DD->cam->GetView().Transpose();
	((ConstantBuffer*)m_pCB)->projection = _DD->cam->GetProj().Transpose();
	((ConstantBuffer*)m_pCB)->lightCol =_DD->light->GetColour();
	((ConstantBuffer*)m_pCB)->lightPos = _DD->light->GetPos();
	((ConstantBuffer*)m_pCB)->ambientCol = m_currentColour + _DD->light->GetAmbCol();
	((ConstantBuffer*)m_pCB)->world = m_worldMat.Transpose();
	((ConstantBuffer*)m_pCB)->rot = m_rotMat.Transpose();
	VBGO::Draw(_DD);
}



