Texture2D<float4> Pos_tex:register(t0);
Texture2D<float4> Nor_tex:register(t1);
Texture2D<float4> Dif_tex:register(t2);
Texture2D<float4> Spec_tex:register(t3);

SamplerState sampleState : register(s0);

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

float4 PS_main( PS_IN input ) : SV_TARGET0
{
	float3 Pos = Pos_tex.Sample(sampleState, input.Tex);
	float3 Nor = Nor_tex.Sample(sampleState, input.Tex);
	float3 Dif = Dif_tex.Sample(sampleState, input.Tex);
	float3 Spec = Spec_tex.Sample(sampleState, input.Tex);
	return float4(Dif,1.f);
	//return float4(0.f,0.f,1.f,0.f);
}