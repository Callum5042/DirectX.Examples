
#include "Header.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

VS_OUTPUT main(float4 Pos : POSITION, float3 colour : COLOUR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//output.Pos = Pos;
	
	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Colour = float4(colour, 1.0f);
	return output;
}