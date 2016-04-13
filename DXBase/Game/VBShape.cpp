//Class that holds the data of the current vertex object and colour
#include "VBShape.h"
#include "VBData.h"
#include "WireCube.h"
#include "SolidCube.h"
#include "WireDiamond.h"
#include "WireCircle.h"
#include "drawdata.h"
#include "gamedata.h"
#include "camera.h"


VBMap VBShape::s_VBHolder;

VBShape::VBShape()
{
	SetDefaultColour(Color(0.2,1,0));
}

VBShape::~VBShape()
{
	
}

void VBShape::AddShapes(ID3D11Device* _GD)
{
	//Create series of vertex objects and store in memory for quick loading
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
	//Creates new vertex object using stored data in memory
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
		//Create new shape or break
	}
}

void VBShape::Tick(GameData* _GD)
{
	VBGO::Tick(_GD);
}
void VBShape::Draw(DrawData* _DD)
{
	Matrix view = _DD->cam->GetView();
	Matrix proj = _DD->cam->GetProj();
	Matrix viewProj = view * proj;
	//Update constant buffer data so colour and position can be changed dynamicly
	/*((ConstantBuffer*)m_pCB)->view = _DD->cam->GetView().Transpose();
	((ConstantBuffer*)m_pCB)->projection = _DD->cam->GetProj().Transpose();
	((ConstantBuffer*)m_pCB)->ambientCol = m_currentColour;
	((ConstantBuffer*)m_pCB)->world = m_worldMat.Transpose();
	((ConstantBuffer*)m_pCB)->rot = m_rotMat.Transpose();*/
	((ConstantBuffer*)m_pCB)->WorldMatrix = m_worldMat;
	((ConstantBuffer*)m_pCB)->InverseTransposeWorldMatrix = m_worldMat.Invert().Transpose();
	((ConstantBuffer*)m_pCB)->WorldViewProjectionMatrix = m_worldMat * viewProj;
	((ConstantBuffer*)m_pCB)->ambientCol = m_currentColour;

	VBGO::Draw(_DD);
}



