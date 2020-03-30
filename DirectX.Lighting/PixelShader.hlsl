
#include "Header.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = 0.0f;// vLightAmbient;
    finalColor += saturate(dot((float3)vLightDir, input.Norm));
    //finalColor += saturate(dot((float3)vLightDir, input.Norm) * vLightColor);
    finalColor.a = 1;
    return finalColor;
}