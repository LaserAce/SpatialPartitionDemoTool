#include "VBShape.h"
#include "VBData.h"
#include "WireCube.h"
#include "SolidCube.h"
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
	VBData* data;

	data = WireCube::InitialiseBuffer(_GD, true);
	s_VBHolder.insert(std::make_pair("WireCube3D", data));

	data = WireCube::InitialiseBuffer(_GD, false);
	s_VBHolder.insert(std::make_pair("WireCube2D", data));

	data = SolidCube::InitialiseBuffer(_GD, false);
	s_VBHolder.insert(std::make_pair("SolidCube2D", data));

	data = WireDiamond::InitialiseBuffer(_GD, true);
	s_VBHolder.insert(std::make_pair("WireDiamond3D", data));

	data = WireDiamond::InitialiseBuffer(_GD, false);
	s_VBHolder.insert(std::make_pair("WireDiamond2D", data));

	data = WireCircle::InitialiseBuffer(_GD, false);
	s_VBHolder.insert(std::make_pair("WireCircle2D", data));
}

void VBShape::InitialiseShape(string _shapeName)
{
	m_pCB = new ConstantBuffer();
	ZeroMemory(m_pCB, sizeof(ConstantBuffer));
	VBMap::iterator it = s_VBHolder.find(_shapeName);
	if (it != s_VBHolder.end())
	{
		m_topology = (*it).second->topology;
		m_numPrims = (*it).second->numPrims;
		m_VertexBuffer = (*it).second->vertexBuffer;
		m_IndexBuffer = (*it).second->indexBuffer;
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
	((ConstantBuffer*)m_pCB)->ambientCol = m_currentColour;
	((ConstantBuffer*)m_pCB)->world = m_worldMat.Transpose();
	((ConstantBuffer*)m_pCB)->rot = m_rotMat.Transpose();
	VBGO::Draw(_DD);
}



