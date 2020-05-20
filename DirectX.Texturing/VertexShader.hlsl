
#include "Header.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Texture = input.Texture;

	return output;
}