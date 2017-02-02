Texture2D<float4> Pos_tex:register(t0);
Texture2D<float4> Nor_tex:register(t1);
Texture2D<float4> Dif_tex:register(t2);

SamplerState sampleState : register(s0);

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

float4 PS_main( PS_IN input ) : SV_TARGET0
{
	float4 Pos = Pos_tex.Sample(sampleState, input.Tex);
	float4 Nor = Nor_tex.Sample(sampleState, input.Tex);
	float4 Dif = Dif_tex.Sample(sampleState, input.Tex);
	float3 Spec = float3(Pos.w, Nor.w, Dif.w);

	//return Dif;
	return float4(Spec,1.0f);
	//return float4(0.f,0.f,1.f,0.f);
}