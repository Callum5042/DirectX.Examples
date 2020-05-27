
#include "Header.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;

	// Transform to homogeneous clip space.
	output.PositionH = mul(float4(input.Position, 1.0f), World);
	output.PositionH = mul(output.PositionH, View);
	output.PositionH = mul(output.PositionH, Projection);

	// Transform to world space.
	output.Position = mul(float4(input.Position, 1.0f), World).xyz;
	output.Normal = mul(float4(input.Normal, 1.0f), World).xyz;

	// Transform texture
	output.Texture = mul(float4(input.Texture, 1.0f, 1.0f), Texture).xy;

	return output;
}