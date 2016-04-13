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

struct AppData
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float4 Color    : COLOR;
	float2 TexCoord : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 PositionWS   : TEXCOORD1;
	float3 NormalWS     : TEXCOORD2;
	float4 Color        : COLOR;
	float2 TexCoord     : TEXCOORD0;
	float4 Position     : SV_Position;
};

VertexShaderOutput EnhancedVertexShader(AppData IN)
{
	VertexShaderOutput OUT;

	OUT.Position = mul(WorldViewProjectionMatrix, float4(IN.Position, 1.0f));
	OUT.PositionWS = mul(WorldMatrix, float4(IN.Position, 1.0f));
	OUT.NormalWS = mul((float3x3)InverseTransposeWorldMatrix, IN.Normal);
	OUT.TexCoord = IN.TexCoord;
	OUT.Color = ambientCol;// IN.Color;
	return OUT;
}
