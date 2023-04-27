struct FragmentIn {
    float2 texCoord : TexCoord;
};

Texture2D    tex          : register(t0);
SamplerState samplerState : register(s1);

float4 main(FragmentIn input) : SV_TARGET {
    return tex.Sample(samplerState, input.texCoord);
}