#include "WireCircle.h"
#include <d3dcompiler.h>
#include "VBData.h"
#include "VBGO.h"

VBData* WireCircle::InitialiseBuffer(ID3D11Device* _pd3d, bool _is3D)
{
	VBData* data = new VBData();
	data->topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	int numVerts = 48;
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
	
	}
	else
	{
		Vector3 startVert = Vector3(0, 1, 0);
		Vector3 vert1 = startVert;
		Vector3 vert2 = vert1;
		float change = 1 / ((float)data->numPrims / 4);
		int xSign = 1;
		int ySign = -1;

		for (int i = 0; i < (int)data->numPrims; ++i)
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

	data->indexBuffer = VBGO::BuildIB(_pd3d, data->numPrims, indices);
	data->vertexBuffer = VBGO::BuildVB(_pd3d, numVerts, m_vertices);

	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	delete[] indices;

	return data;
}

