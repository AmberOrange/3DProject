Texture2D shaderTexture : register(t0);
SamplerState sampleState : register(s0);

struct PS_IN
{
	float4 VSPos : SV_POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 WPos : POSITION;
};

float4 PS_main(PS_IN input) : SV_TARGET
{
	return shaderTexture.Sample(sampleState,input.Tex);
	//return float4(normalize(input.WPos),1.0f);
}