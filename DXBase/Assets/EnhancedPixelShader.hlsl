Texture2D		Texture : register(t0);
sampler	Sampler : register(s0);

cbuffer PerObject : register(b0)
{
	matrix WorldMatrix;
	matrix InverseTransposeWorldMatrix;
	matrix WorldViewProjectionMatrix;
	float4 ambientCol;
		/*
		matrix World;
		matrix View;
		matrix Projection;
		matrix rot;
		float4 ambientCol;*/
}

struct PixelShaderInput
{
	float4 PositionWS   : TEXCOORD1;
	float3 NormalWS     : TEXCOORD2;
	float4 Color        : COLOR;
	float2 TexCoord     : TEXCOORD0;
};

float4 EnhancedPixelShader(PixelShaderInput IN) : SV_TARGET
{
	return IN.Color;
}

