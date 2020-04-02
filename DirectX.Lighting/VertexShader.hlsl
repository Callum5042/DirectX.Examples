
#include "Header.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = mul(float4(input.Normal, 1), World).xyz;

	return output;
}