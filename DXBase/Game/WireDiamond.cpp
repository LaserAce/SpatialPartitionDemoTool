#include "WireDiamond.h"
#include <d3dcompiler.h>
#include "VBData.h"
#include "VBGO.h"

VBData* WireDiamond::InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D)
{
	VBData* data = new VBData();
	data->topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	int numVerts = 8;
	if (_is3D)
	{
		numVerts *= 3;
	}
	data->numPrims = numVerts / 2;

	data->indexBuffer = NULL;
	data->vertexBuffer = NULL;

	Color white = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//calculate number of vertices and primatives

	
	myVertex* m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = static_cast<WORD>(i);
		m_vertices[i].texCoord = Vector2::One;
	}

	int vert = 0;
	if (_is3D)
	{
		//1
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 1, 0);
		m_vertices[vert++].Pos = Vector3(0, 1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 1, 0);
		m_vertices[vert++].Pos = Vector3(1, 0, 0);
		//2
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 1, 0);
		m_vertices[vert++].Pos = Vector3(0, 1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 1, 0);
		m_vertices[vert++].Pos = Vector3(-1, 0, 0);
		//3
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, 1, 1);
		m_vertices[vert++].Pos = Vector3(0, 1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, 1, 1);
		m_vertices[vert++].Pos = Vector3(0, 0, 1);
		//4
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, 1, -1);
		m_vertices[vert++].Pos = Vector3(0, 1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, 1, -1);
		m_vertices[vert++].Pos = Vector3(0, 0, -1);
		//5
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, -1, 0);
		m_vertices[vert++].Pos = Vector3(0, -1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, -1, 0);
		m_vertices[vert++].Pos = Vector3(1, 0, 0);
		//6
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, -1, 0);
		m_vertices[vert++].Pos = Vector3(0, -1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, -1, 0);
		m_vertices[vert++].Pos = Vector3(-1, 0, 0);
		//7
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, -1, 1);
		m_vertices[vert++].Pos = Vector3(0, -1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, -1, 1);
		m_vertices[vert++].Pos = Vector3(0, 0, 1);
		//8
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, -1, -1);
		m_vertices[vert++].Pos = Vector3(0, -1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(0, -1, -1);
		m_vertices[vert++].Pos = Vector3(0, 0, -1);
		//9
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 0, 1);
		m_vertices[vert++].Pos = Vector3(0, 0, 1);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 0, 1);
		m_vertices[vert++].Pos = Vector3(1, 0, 0);
		//10
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 0, -1);
		m_vertices[vert++].Pos = Vector3(1, 0, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 0, -1);
		m_vertices[vert++].Pos = Vector3(0, 0, -1);
		//11
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 0, -1);
		m_vertices[vert++].Pos = Vector3(0, 0, -1);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 0, -1);
		m_vertices[vert++].Pos = Vector3(-1, 0, 0);
		//12
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 0, 1);
		m_vertices[vert++].Pos = Vector3(-1, 0, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 0, 1);
		m_vertices[vert++].Pos = Vector3(0, 0, 1);
	}
	else
	{
		//1
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 1, 0);
		m_vertices[vert++].Pos = Vector3(0, 1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, 1, 0);
		m_vertices[vert++].Pos = Vector3(1, 0, 0);
		//2
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, -1, 0);
		m_vertices[vert++].Pos = Vector3(1, 0, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(1, -1, 0);
		m_vertices[vert++].Pos = Vector3(0, -1, 0);
		//3
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, -1, 0);
		m_vertices[vert++].Pos = Vector3(0, -1, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, -1, 0);
		m_vertices[vert++].Pos = Vector3(-1, 0, 0);
		//4
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 1, 0);
		m_vertices[vert++].Pos = Vector3(-1, 0, 0);
		m_vertices[vert].Color = white;
		m_vertices[vert].Norm = Vector3(-1, 1, 0);
		m_vertices[vert++].Pos = Vector3(0, 1, 0);
	}

	data->indexBuffer = VBGO::BuildIB(_pd3d, data->numPrims, indices);
	data->vertexBuffer = VBGO::BuildVB(_pd3d, numVerts, m_vertices);

	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	delete[] indices;

	return data;
}

