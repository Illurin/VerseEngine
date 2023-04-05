struct FragmentIn {
	float2 texCoord : TexCoord;
};

//Texture2D tex : register(t1, space0);
//SamplerState samplerState : register(s2, space0);

float4 main(FragmentIn input) : SV_Target0 {
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}