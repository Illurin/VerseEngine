struct FragmentIn {
	float2 texCoord : TexCoord;
};

Texture2D tex : register(t1, space0);
SamplerState samplerState : register(s2, space0);

float4 main(FragmentIn input) : SV_Target0 {
	return tex.Sample(samplerState, input.texCoord);
}