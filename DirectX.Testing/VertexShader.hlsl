
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

VS_OUTPUT main(float4 Pos : POSITION, float3 colour : COLOUR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = Pos;
	output.Color = float4(colour, 1.0f);
	return output;
}