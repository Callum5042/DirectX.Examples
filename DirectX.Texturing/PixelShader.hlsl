
#include "Header.hlsli"

float4 main(PS_INPUT pin) : SV_TARGET
{
	return gDiffuseMap.Sample(samplerAnisotropic, pin.Texture);
}