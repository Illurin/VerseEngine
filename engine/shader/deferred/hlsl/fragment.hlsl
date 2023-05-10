#include "..\..\include\hlsl\common.hlsl"
#include "..\..\include\hlsl\lighting_util.hlsl"

struct FragmentIn {
    float2 texCoord : TexCoord;
};

Texture2D                     normal        : register(t0, space0);
Texture2D                     worldPos      : register(t1, space0);
Texture2D                     diffuseAlbedo : register(t2, space0);
Texture2D                     material      : register(t3, space0);
SamplerState                  samplerState  : register(s4, space0);

ConstantBuffer<PassConstants> passConstants : register(b0, space1);
TextureCube                   skybox        : register(t1, space1);
SamplerState                  skyboxSampler : register(s2, space1);

float4 main(FragmentIn input) : SV_Target0 {
    float3 normalW      = normal.Sample(samplerState, input.texCoord).rgb;
    float3 posW         = worldPos.Sample(samplerState, input.texCoord).rgb;
    float4 diffuse      = diffuseAlbedo.Sample(samplerState, input.texCoord);
    float4 materialProp = material.Sample(samplerState, input.texCoord);
    
    float3 toEye = normalize(passConstants.eyePos.xyz - posW);
    
    float3 lightingResult = (float3) 0.0f;
    
    MaterialConstants materialConstants;
    materialConstants.diffuseAlbedo = diffuse;
    materialConstants.fresnelR0 = materialProp.rgb;
    materialConstants.roughness = materialProp.a;
    
    uint i = 0;
    [unroll] for (i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++) {
        Light light;
        LightConstants lightConstants = passConstants.lights[i];
        light.strength = lightConstants.intensity * lightConstants.color;
        light.direction = lightConstants.direction;
        lightingResult += ComputeDirectionalLight(light, normalW, toEye, materialConstants);
    }
    
    [unroll] for (i = 0; i < MAX_POINT_LIGHTS; i++) {
        Light light;
        LightConstants lightConstants = passConstants.lights[i + MAX_DIRECTIONAL_LIGHTS];
        light.strength = lightConstants.intensity * lightConstants.color;
        light.position = lightConstants.position;
        light.attenuation = lightConstants.attenuation;
        lightingResult += ComputePointLight(light, posW, normalW, toEye, materialConstants);
    }
    
    [unroll] for (i = 0; i < MAX_SPOT_LIGHTS; i++) {
        Light light;
        LightConstants lightConstants = passConstants.lights[i + MAX_DIRECTIONAL_LIGHTS + MAX_POINT_LIGHTS];
        light.strength = lightConstants.intensity * lightConstants.color;
        light.direction = lightConstants.direction;
        light.position = lightConstants.position;
        light.attenuation = lightConstants.attenuation;
        light.spotPower = lightConstants.spotPower;
        lightingResult += ComputeSpotLight(light, posW, normalW, toEye, materialConstants);
    }
    
    float3 reflectionVec = reflect(-toEye, normalW);
    float3 reflectionColor = skybox.Sample(skyboxSampler, reflectionVec).rgb;
    reflectionColor *= (1.0f - materialConstants.roughness) * SchlickFresnel(materialConstants.fresnelR0, normalW, reflectionVec);
    
    float3 litColor = diffuse.rgb * (passConstants.ambientLight.rgb + lightingResult)/* * reflectionColor*/;
    return float4(litColor, diffuse.a);
}