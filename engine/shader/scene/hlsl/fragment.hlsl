#include "..\..\include\hlsl\common.hlsl"
#include "..\..\include\hlsl\lighting_util.hlsl"

struct FragmentIn {
    float3 normal : Normal;
	float2 texCoord : TexCoord;
    float3 posW : WorldPos;
};

ConstantBuffer<MaterialConstants> materialConstants : register(b0, space1);
Texture2D                         tex               : register(t1, space1);
SamplerState                      samplerState      : register(s2, space1);

                           
TextureCube  skybox        : register(t1, space2);
SamplerState skyboxSampler : register(s2, space2);

float4 main(FragmentIn input) : SV_Target0 {
    float4 diffuse = materialConstants.diffuseAlbedo * tex.Sample(samplerState, input.texCoord);
    float3 normal = normalize(input.normal);
    float3 toEye = normalize(passConstants.eyePos.xyz - input.posW);
    
    Light light;
    light.position = float3(3.0f, 7.0f, 0.0f);
    light.strength = float3(0.8f, 0.8f, 1.0f);
    light.attenuation = 0.3f;
    float3 lightingResult = ComputePointLight(light, input.posW, normal, toEye, materialConstants);
    
    float3 litColor = diffuse.rgb * (passConstants.ambientLight + lightingResult);
    return float4(litColor, diffuse.a);
}