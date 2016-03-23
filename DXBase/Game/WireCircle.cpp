#include "WireCircle.h"
#include <d3dcompiler.h>
#include "gamedata.h"
#include "drawdata.h"

VBData WireCircle::InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D)
{
	Color white = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//calculate number of vertices and primatives

	int numVerts = 48;
	if (_is3D)
	{
		numVerts *= 3;
	}
	m_numPrims = numVerts / 2;
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
		/*//1
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
		m_vertices[vert++].Pos = Vector3(0, 0, 1);*/
	}
	else
	{
		Vector3 startVert = Vector3(0, 1, 0);
		Vector3 vert1 = startVert;
		Vector3 vert2 = vert1;
		float change = 1 / ((float)m_numPrims / 4);
		int xSign = 1;
		int ySign = -1;

		for (int i = 0; i < (int)m_numPrims; ++i)
		{
			vert2.x = vert2.x + (change * xSign);
			if (abs(vert2.x + (change * xSign)) > 1 + (change / 2))
			{
				xSign *= -1;
			}
			vert2.y = vert2.y + (change * ySign);
			if (abs(vert2.y + (change * ySign)) > 1 + (change / 2))
			{
				ySign *= -1;
			}

			float angle = atan2(-vert1.x, vert1.y);
			Matrix rot = Matrix::CreateRotationZ(angle);
			Vector3 finalvert = startVert;
			finalvert = finalvert.Transform(finalvert, rot);

			m_vertices[vert].Color = white;
			m_vertices[vert].Norm = Vector3(0, 0, 1);
			m_vertices[vert++].Pos = finalvert;

			angle = atan2(-vert2.x, vert2.y);
			rot = Matrix::CreateRotationZ(angle);
			finalvert = startVert;
			finalvert = finalvert.Transform(finalvert, rot);

			m_vertices[vert].Color = white;
			m_vertices[vert].Norm = Vector3(0, 0, 1);
			m_vertices[vert++].Pos = finalvert;

			vert1 = vert2;
		}	
	}

	//carry out some kind of transform on these vertices to make this object more interesting
	Transform();

	BuildIB(_pd3d, indices);
	BuildVB(_pd3d, numVerts, m_vertices);

	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	delete[] indices;

	VBData dataReturn;
	dataReturn.first = m_numPrims;
	dataReturn.second.push_back(m_VertexBuffer);
	dataReturn.second.push_back(m_IndexBuffer);
	return dataReturn;
}

void WireCircle::Tick(GameData* _GD)
{
	VBGO::Tick(_GD);
}

void WireCircle::Draw(DrawData* _DD)
{
	VBGO::Draw(_DD);
}

