
#include "Header.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

VS_OUTPUT main(float4 pos : POSITION)
{
	VS_OUTPUT output;

	output.Pos = mul(pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	return output;
}