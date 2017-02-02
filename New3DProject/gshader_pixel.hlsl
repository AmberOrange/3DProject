Texture2D shaderTexture : register(t0);
SamplerState sampleState : register(s0);

struct PS_IN
{
	float4 VSPos : SV_POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float3 WPos : POSITION;
};


// float4 is way overkill for this, optimize in the future
struct PS_OUT
{
	float4 Pos : SV_TARGET0;
	float4 Nor : SV_TARGET1;
	float4 Dif : SV_TARGET2;
};

PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;
	
	float4 Spec = float4(0.0f,1.0f,0.0f,0.0f);

	output.Pos = float4(input.WPos, Spec.x);
	output.Nor = float4(normalize(input.Nor), Spec.y);
	output.Dif = float4(shaderTexture.Sample(sampleState,input.Tex).xyz, Spec.z);

	return output;
}