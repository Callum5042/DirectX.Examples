
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix Texture;
}

struct VS_INPUT
{
	float4 Position : POSITION;
	float2 Texture : TEXCOORD;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 Texture : TEXCOORD;
};

Texture2D gDiffuseMap : register(t0);

SamplerState samplerAnisotropic : register(s0);