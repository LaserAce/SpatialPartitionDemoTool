#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "VBCube.h"

class Sphere : public VBCube
{
public:
	Sphere(){};


protected:
	virtual void Transform()
	{
		for (unsigned int i = 0; i<m_numPrims * 3; i++)
		{
			m_vertices[i].Color *= ((i / 3) % 2) ? 1.0f : 0.5f;
			m_vertices[i].Color.w = 1.0f;

			Vector3 vertPos = m_vertices[i].Pos;
			Vector3 spherePos = m_vertices[i].Pos;

			spherePos.Normalize();

			Vector3 newPos = 10.0f *spherePos;

			m_vertices[i].Pos = newPos;
		}
	}

};

#endif