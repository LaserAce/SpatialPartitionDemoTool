//--------------------------------------------------------------------------------------
// A basic shader with colour and texture fetch and a simple light
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix rot;
	float4 ambientCol;
}

//--------------------------------------------------------------------------------------
// texture
//--------------------------------------------------------------------------------------
Texture2D		myTexture : register( t0 );
SamplerState	mySampler : register( s0 );

//--------------------------------------------------------------------------------------
// input structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
	float4 Norm : NORMAL;
    float4 Color : COLOR;
	float2 texCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 worldPos: POSITION;
	float4 Norm : NORMAL;
    float4 Color : COLOR;
	float2 texCoord : TEXCOORD;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

	output.worldPos = mul( input.Pos, World );
    output.Pos = mul( output.worldPos, View );
    output.Pos = mul( output.Pos, Projection );

    output.Norm = mul( input.Norm, rot );

    output.Color = input.Color;

	output.texCoord = input.texCoord;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	return ambientCol;
}
