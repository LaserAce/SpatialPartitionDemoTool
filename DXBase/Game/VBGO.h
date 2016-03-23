#ifndef _VB_GO_H_
#define _VB_GO_H_
#include "gameobject.h"
#include "ConstantBuffer.h"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;
using std::pair;

//the base Game Object drawn using a Vertex and Index Buffer
//all of the main aspects of drawing it have a default which is pointed to by a static pointer
//this is only used if the version for this object is not set to nullptr

//Use to store buffer information
typedef pair < UINT, vector<ID3D11Buffer*> > VBData;
//Used to store buffer information with identification string
typedef map<string, VBData> VBMap;

class VBGO :public GameObject
{
public:
	VBGO();
	virtual ~VBGO();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	static void Init(ID3D11Device* _GD);
	static void CleanUp();

	static void UpdateConstantBuffer(DrawData* _DD);

	void SetWireframe(bool boolean, ID3D11Device* _GD);

	static ID3D11Device* s_pd3dDevice;


protected:

	//Direct X information for the model for this GameObject
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	UINT m_numPrims;

	D3D_PRIMITIVE_TOPOLOGY m_topology;

	//default vertexshader
	static ID3D11VertexShader*     s_pVertexShader;
	//default vertex layout
	static ID3D11InputLayout*      s_pVertexLayout;
	//default pixelshader
	static ID3D11PixelShader*      s_pPixelShader;
	//default texture (white square)
	static ID3D11ShaderResourceView* s_pTextureRV;
	//deafult const buffer
	static ID3D11Buffer*           s_pConstantBuffer;	//GPU side
	static ConstantBuffer*			s_pCB;				//CPU side
	//default sampler
	static ID3D11SamplerState*		s_pSampler;
	//default raster state
	static ID3D11RasterizerState*  s_pRasterState;

	//my vertexshader
	ID3D11VertexShader*     m_pVertexShader;
	//my vertex layout
	ID3D11InputLayout*      m_pVertexLayout;
	//my pixelshader
	ID3D11PixelShader*      m_pPixelShader;
	//my texture
	ID3D11ShaderResourceView* m_pTextureRV;
	//my const buffer
	ID3D11Buffer*           m_pConstantBuffer; //GPU side
	void*					m_pCB;//CPU side
	//my sampler
	ID3D11SamplerState*		m_pSampler;
	//my raster state
	ID3D11RasterizerState*  m_pRasterState;

	//once populated build an Index Buffer
	void BuildIB(ID3D11Device* _GD,void* _indices);

	//once populated build a Vertex Buffer
	void BuildVB(ID3D11Device* _GD,int _numVerts, void* _vertices);

	static HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	bool wireframe = false;

};

#endif