#include "..\..\include\hlsl\common.hlsl"
#include "..\..\include\hlsl\lighting_util.hlsl"

struct FragmentIn {
    float3 normal   : Normal;
    float3 tangent  : Tangent;
    float2 texCoord : TexCoord;
    float3 posW     : WorldPos;
};

ConstantBuffer<MaterialConstants> materialConstants : register(b0, space1);
Texture2D                         diffuseMap        : register(t1, space1);
SamplerState                      diffuseMapSampler : register(s2, space1);
Texture2D                         normalMap         : register(t3, space1);
SamplerState                      normalMapSampler  : register(s4, space1);

ConstantBuffer<PassConstants> passConstants : register(b0, space2);
TextureCube                   skybox        : register(t1, space2);
SamplerState                  skyboxSampler : register(s2, space2);

float4 main(FragmentIn input) : SV_Target0 {
    float4 diffuse = materialConstants.diffuseAlbedo * diffuseMap.Sample(diffuseMapSampler, input.texCoord);
    
    if (diffuse.a <= 0.0f) discard;
    
    float3 sampledNormal = normalMap.Sample(normalMapSampler, input.texCoord).rgb;
    //float3 normalW = GetWorldSpaceNormal(sampledNormal, normalize(input.normal), input.tangent);
    float3 normalW = normalize(input.normal);
    
    float3 toEye = normalize(passConstants.eyePos.xyz - input.posW);
    
    float3 lightingResult = (float3) 0.0f;
    
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
        lightingResult += ComputePointLight(light, input.posW, normalW, toEye, materialConstants);
    }
    
    [unroll] for (i = 0; i < MAX_SPOT_LIGHTS; i++) {
        Light light;
        LightConstants lightConstants = passConstants.lights[i + MAX_DIRECTIONAL_LIGHTS + MAX_POINT_LIGHTS];
        light.strength = lightConstants.intensity * lightConstants.color;
        light.direction = lightConstants.direction;
        light.position = lightConstants.position;
        light.attenuation = lightConstants.attenuation;
        light.spotPower = lightConstants.spotPower;
        lightingResult += ComputeSpotLight(light, input.posW, normalW, toEye, materialConstants);
    }
    
    float3 reflectionVec = reflect(-toEye, normalW);
    float3 reflectionColor = skybox.Sample(skyboxSampler, reflectionVec).rgb;
    reflectionColor *= (1.0f - materialConstants.roughness) * SchlickFresnel(materialConstants.fresnelR0, normalW, reflectionVec);
    
    float3 litColor = diffuse.rgb * (passConstants.ambientLight.rgb + lightingResult)/* * reflectionColor*/;
    return float4(litColor, diffuse.a);
}