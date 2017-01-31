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
	float4 Spec : SV_TARGET3;
};

PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;
	
	output.Pos = float4(input.WPos, 1.0f);
	output.Nor = float4(normalize(input.Nor), 1.0f);
	output.Dif = shaderTexture.Sample(sampleState,input.Tex);
	//output.Dif = float4(1.0f,0.0f,0.0f,1.0f);
	output.Spec = float4(0.0f, 0.0f, 0.0f, 0.0f);	// Change in the future

	return output;
}