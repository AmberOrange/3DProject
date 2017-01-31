cbuffer Matrixes : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
}

struct VS_IN
{
	float3 Pos : POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VS_OUT
{
	float4 VSPos : SV_POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 WPos : POSITION;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.VSPos = mul(float4(input.Pos, 1.0f), World);
	output.VSPos = mul(output.VSPos, View);
	output.VSPos = mul(output.VSPos, Projection);

	output.Nor = normalize(mul(float4(input.Nor, 1.0f), World)).xyz;

	output.WPos = mul(float4(input.Pos, 1.0f), World).xyz;

	output.Tex = input.Tex;

	return output;
}