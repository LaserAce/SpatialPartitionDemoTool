#include "texturecube.h"
#include <d3dcompiler.h>
#include "gamedata.h"
#include "drawdata.h"
#include "DDSTextureLoader.h"
#include <iostream>

VBData TextureCube::InitialiseBuffer(int _size, ID3D11Device* _pd3d)
{
	Color white = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//white *= 0.5f;
	//white.w = 1.0f;
	//calculate number of vertices and primatives
	int numVerts = 6 * 6;
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = static_cast<WORD>(i);
		m_vertices[i].texCoord = Vector2::One;
		m_vertices[i].Color = white;
	}

	int vert = 0;

	int i = -5;
	int j = -5;
	int k = -5;
	int s = 10;

	//Top
	m_vertices[vert].texCoord = Vector2((float)0,(float)0);
	m_vertices[vert].Texture = 0;
	m_vertices[vert++].Pos = Vector3((float)i, (float)(k + s), (float)j);
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 0;
	m_vertices[vert++].Pos = Vector3((float)i, (float)(k + s), (float)(j + s));
	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 0;
	m_vertices[vert++].Pos = Vector3((float)(i + s), (float)(k + s), (float)j);

	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 0;
	m_vertices[vert++].Pos = Vector3((float)(i + s), (float)(k + s), (float)j);
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 0;
	m_vertices[vert++].Pos = Vector3((float)i, (float)(k + s), (float)(j + s));
	m_vertices[vert].texCoord = Vector2((float)1, (float)1);
	m_vertices[vert].Texture = 0;
	m_vertices[vert++].Pos = Vector3((float)(i + s), (float)(k + s), (float)(j + s));

	//back
	m_vertices[vert].texCoord = Vector2((float)1, (float)1);
	m_vertices[vert].Texture = 1;
	m_vertices[vert++].Pos = Vector3((float)i, (float)j, (float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 1;
	m_vertices[vert++].Pos = Vector3((float)(i + s), (float)j, (float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 1;
	m_vertices[vert++].Pos = Vector3((float)i, (float)(j + s), (float)(k + s));

	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 1;
	m_vertices[vert++].Pos = Vector3((float)(i + s), (float)j, (float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)0, (float)0);
	m_vertices[vert].Texture = 1;
	m_vertices[vert++].Pos = Vector3((float)(i + s), (float)(j + s), (float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 1;
	m_vertices[vert++].Pos = Vector3((float)i, (float)(j + s), (float)(k + s));

	//right
	m_vertices[vert].texCoord = Vector2((float)1, (float)1);
	m_vertices[vert].Texture = 2;
	m_vertices[vert++].Pos = Vector3((float)(k + s), (float)i, (float)j);
	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 2;
	m_vertices[vert++].Pos = Vector3((float)(k + s), (float)(i + s), (float)j);
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 2;
	m_vertices[vert++].Pos = Vector3((float)(k + s), (float)i, (float)(j + s));

	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 2;
	m_vertices[vert++].Pos = Vector3((float)(k + s), (float)(i + s), (float)j);
	m_vertices[vert].texCoord = Vector2((float)0, (float)0);
	m_vertices[vert].Texture = 2;
	m_vertices[vert++].Pos = Vector3((float)(k + s), (float)(i + s), (float)(j + s));
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 2;
	m_vertices[vert++].Pos = Vector3((float)(k + s), (float)i, (float)(j + s));

	//Bottom
	m_vertices[vert].Texture = 5;
	m_vertices[vert++].Pos = Vector3((float)j, -(float)(k + s), (float)i);
	m_vertices[vert].Texture = 5;
	m_vertices[vert++].Pos = Vector3((float)(j + s), -(float)(k + s), (float)i);
	m_vertices[vert].Texture = 5;
	m_vertices[vert++].Pos = Vector3((float)j, -(float)(k + s), (float)(i + s));

	m_vertices[vert].Texture = 5;
	m_vertices[vert++].Pos = Vector3((float)j, -(float)(k + s), (float)(i + s));
	m_vertices[vert].Texture = 5;
	m_vertices[vert++].Pos = Vector3((float)(j + s), -(float)(k + s), (float)i);
	m_vertices[vert].Texture = 5;
	m_vertices[vert++].Pos = Vector3((float)(j + s), -(float)(k + s), (float)(i + s));

	//front
	m_vertices[vert].texCoord = Vector2((float)1, (float)1);
	m_vertices[vert].Texture = 3;
	m_vertices[vert++].Pos = Vector3((float)j, (float)i, -(float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 3;
	m_vertices[vert++].Pos = Vector3((float)j, (float)(i + s), -(float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 3;
	m_vertices[vert++].Pos = Vector3((float)(j + s), (float)i, -(float)(k + s));

	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 3;
	m_vertices[vert++].Pos = Vector3((float)j, (float)(i + s), -(float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)0, (float)0);
	m_vertices[vert].Texture = 3;
	m_vertices[vert++].Pos = Vector3((float)(j + s), (float)(i + s), -(float)(k + s));
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 3;
	m_vertices[vert++].Pos = Vector3((float)(j + s), (float)i, -(float)(k + s));

	//left
	m_vertices[vert].texCoord = Vector2((float)1, (float)1);
	m_vertices[vert].Texture = 4;
	m_vertices[vert++].Pos = Vector3(-(float)(k + s), (float)j, (float)i);
	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 4;
	m_vertices[vert++].Pos = Vector3(-(float)(k + s), (float)j, (float)(i + s));
	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 4;
	m_vertices[vert++].Pos = Vector3(-(float)(k + s), (float)(j + s), (float)i);

	m_vertices[vert].texCoord = Vector2((float)0, (float)1);
	m_vertices[vert].Texture = 4;
	m_vertices[vert++].Pos = Vector3(-(float)(k + s), (float)j, (float)(i + s));
	m_vertices[vert].texCoord = Vector2((float)0, (float)0);
	m_vertices[vert].Texture = 4;
	m_vertices[vert++].Pos = Vector3(-(float)(k + s), (float)(j + s), (float)(i + s));
	m_vertices[vert].texCoord = Vector2((float)1, (float)0);
	m_vertices[vert].Texture = 4;
	m_vertices[vert++].Pos = Vector3(-(float)(k + s), (float)(j + s), (float)i);

	




	//carry out some kind of transform on these vertices to make this object more interesting
	Transform();

	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i<m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}

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

void TextureCube::Tick(GameData* _GD)
{
	VBGO::Tick(_GD);
}
void TextureCube::Draw(DrawData* _DD)
{
	VBGO::Draw(_DD);
}

