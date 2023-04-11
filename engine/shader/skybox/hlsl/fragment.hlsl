struct FragmentIn {
    float3 posL : LocalPos;
};

TextureCube  skybox        : register(t1);
SamplerState skyboxSampler : register(s2);

float4 main(FragmentIn input) : SV_Target0 {
    return skybox.Sample(skyboxSampler, input.posL);
}