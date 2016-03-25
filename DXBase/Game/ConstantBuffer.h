//constant buffer for the shader
#ifndef _CONSTBUFFER_H_
#define _CONSTBUFFER_H_

#include <windows.h>
#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX;
using namespace SimpleMath;

//as passing to GPU needs to be correctly memory aligned
__declspec(align(16))
struct ConstantBuffer
{
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	Matrix	world;
	Matrix	view;
	Matrix	projection;
	Matrix	rot;
	Color	ambientCol;
};
#endif