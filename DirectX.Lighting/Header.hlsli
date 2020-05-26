
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix Texture;
	int UseAlpha;
}

struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gOpacityMap : register(t1);

SamplerState samplerAnisotropic : register(s0);