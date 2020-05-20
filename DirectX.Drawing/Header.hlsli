
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
	float4 Position : POSITION;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
};