#include "VBCube.h"
#include <d3dcompiler.h>
#include "gamedata.h"
#include "drawdata.h"

VBData VBCube::InitialiseBuffer(int _size, ID3D11Device* _pd3d)
{
	_size;
	Color white = Color(1.0f, 1.0f, 1.0f, 1.0f);
	white *= 0.5f;
	white.w = 1.0f;
	//calculate number of vertices and primatives
	/*int numVerts = 6 * 6 * (_size - 1) * (_size - 1);
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];*/
	int numVerts = 24;
	m_numPrims = numVerts / 2;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	/*for (int i = 0; i < numVerts; i++)
	{
		indices[i] = static_cast<WORD>(i);
		m_vertices[i].texCoord = Vector2::One;
	}*/
	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = static_cast<WORD>(i);
		m_vertices[i].texCoord = Vector2::One;
	}

	//in each loop create the two traingles for the matching sub-square on each of the six faces
	/*int vert = 0;
	for (int i = -(_size - 1) / 2; i<(_size - 1) / 2; i++)
	{
		for (int j = -(_size - 1) / 2; j<(_size - 1) / 2; j++)
		{
			//top
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)i, 0.5f * (float)(_size - 1), (float)j);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)i, 0.5f * (float)(_size - 1), (float)(j + 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(i + 1), 0.5f * (float)(_size - 1), (float)j);

			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(i + 1), 0.5f * (float)(_size - 1), (float)j);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)i, 0.5f * (float)(_size - 1), (float)(j + 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(i + 1), 0.5f * (float)(_size - 1), (float)(j + 1));

			//back
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)i, (float)j, 0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(i + 1), (float)j, 0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)i, (float)(j + 1), 0.5f * (float)(_size - 1));

			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(i + 1), (float)j, 0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(i + 1), (float)(j + 1), 0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)i, (float)(j + 1), 0.5f * (float)(_size - 1));

			//right
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(0.5f * (float)(_size - 1), (float)i, (float)j);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(0.5f * (float)(_size - 1), (float)(i + 1), (float)j);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(0.5f * (float)(_size - 1), (float)i, (float)(j + 1));

			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(0.5f * (float)(_size - 1), (float)(i + 1), (float)j);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(0.5f * (float)(_size - 1), (float)(i + 1), (float)(j + 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(0.5f * (float)(_size - 1), (float)i, (float)(j + 1));

			//Bottom
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)j, -0.5f * (float)(_size - 1), (float)i);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(j + 1), -0.5f * (float)(_size - 1), (float)i);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)j, -0.5f * (float)(_size - 1), (float)(i + 1));

			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)j, -0.5f * (float)(_size - 1), (float)(i + 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(j + 1), -0.5f * (float)(_size - 1), (float)i);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(j + 1), -0.5f * (float)(_size - 1), (float)(i + 1));

			//front
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)j, (float)i, -0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)j, (float)(i + 1), -0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(j + 1), (float)i, -0.5f * (float)(_size - 1));

			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)j, (float)(i + 1), -0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(j + 1), (float)(i + 1), -0.5f * (float)(_size - 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3((float)(j + 1), (float)i, -0.5f * (float)(_size - 1));

			//left
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(-0.5f * (float)(_size - 1), (float)j, (float)i);
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(-0.5f * (float)(_size - 1), (float)j, (float)(i + 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(-0.5f * (float)(_size - 1), (float)(j + 1), (float)i);

			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(-0.5f * (float)(_size - 1), (float)j, (float)(i + 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(-0.5f * (float)(_size - 1), (float)(j + 1), (float)(i + 1));
			m_vertices[vert].Color = white;
			m_vertices[vert++].Pos = Vector3(-0.5f * (float)(_size - 1), (float)(j + 1), (float)i);

		}
	}*/
	int vert = 0;
	//1
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, 1, -1);
	m_vertices[vert++].Pos = Vector3(-1, 1,-1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, 1, -1);
	m_vertices[vert++].Pos = Vector3(1, 1, -1);
	//2
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, 1, 0);
	m_vertices[vert++].Pos = Vector3(1, 1, -1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, 1, 0);
	m_vertices[vert++].Pos = Vector3(1, 1, 1);
	//3
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, 1, 1);
	m_vertices[vert++].Pos = Vector3(1, 1, 1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, 1, 1);
	m_vertices[vert++].Pos = Vector3(-1, 1, 1);
	//4
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, 1, 0);
	m_vertices[vert++].Pos = Vector3(-1, 1, 1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, 1, 0);
	m_vertices[vert++].Pos = Vector3(-1, 1, -1);
	//5
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, -1, -1);
	m_vertices[vert++].Pos = Vector3(-1, -1, -1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, -1, -1);
	m_vertices[vert++].Pos = Vector3(1, -1, -1);
	//6
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, -1, 0);
	m_vertices[vert++].Pos = Vector3(1, -1, -1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, -1, 0);
	m_vertices[vert++].Pos = Vector3(1, -1, 1);
	//7
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, -1, 1);
	m_vertices[vert++].Pos = Vector3(1, -1, 1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(0, -1, 1);
	m_vertices[vert++].Pos = Vector3(-1, -1, 1);
	//8
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, -1, 0);
	m_vertices[vert++].Pos = Vector3(-1, -1, 1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, -1, 0);
	m_vertices[vert++].Pos = Vector3(-1, -1, -1);
	//9
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, 0, -1);
	m_vertices[vert++].Pos = Vector3(-1, 1, -1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, 0, -1);
	m_vertices[vert++].Pos = Vector3(-1, -1, -1);
	//10
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, 0, -1);
	m_vertices[vert++].Pos = Vector3(1, 1, -1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, 0, -1);
	m_vertices[vert++].Pos = Vector3(1, -1, -1);
	//11
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, 0, 1);
	m_vertices[vert++].Pos = Vector3(1, 1, 1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(1, 0, 1);
	m_vertices[vert++].Pos = Vector3(1, -1, 1);
	//12
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, 0, 1);
	m_vertices[vert++].Pos = Vector3(-1, 1, 1);
	m_vertices[vert].Color = white;
	m_vertices[vert].Norm = Vector3(-1, 0, 1);
	m_vertices[vert++].Pos = Vector3(-1, -1, 1);
	
	

	//carry out some kind of transform on these vertices to make this object more interesting
	Transform();

	//calculate the normals for the basic lighting in the base shader
	/*for (int i = 0; i<(int)m_numPrims; i++)
	{
		int V1 = 3 * i;
		int V2 = 3 * i + 1;
		int V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}*/


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

void VBCube::Tick(GameData* _GD)
{	
	VBGO::Tick(_GD);
}
void VBCube::Draw(DrawData* _DD)
{
	VBGO::Draw(_DD);
}

