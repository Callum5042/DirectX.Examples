
#include "Header.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    input.Norm = normalize(input.Norm);
    float4 finalColor = dot((float3)vLightDir, input.Norm) * vLightColor;
    finalColor.a = 1;
    return finalColor;
}