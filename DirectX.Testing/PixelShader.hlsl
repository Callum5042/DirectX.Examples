
#include "Header.hlsli"

float4 main(VS_OUTPUT input) : SV_Target
{
	return textureDiffuse.Sample(linearSampler, input.Texture);
}