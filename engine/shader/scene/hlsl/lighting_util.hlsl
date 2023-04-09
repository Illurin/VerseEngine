#include "common.hlsl"

struct MaterialConstants {
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float shiniess;
};

ConstantBuffer<MaterialConstants> materialConstants : register(b0, space1);

struct Light {
    float3 strength;
    float3 direction;
    float3 position;
    float spotPower;
    float attenuation;
};

float3 SchlickFresnel(float3 fresnelR0, float3 normal, float3 lightVec) {
    float cosIncidentAngle = saturate(dot(normal, lightVec));
    float f0 = 1.0f - cosIncidentAngle;
    return fresnelR0 + (1.0f - fresnelR0) * pow(f0, 5);
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye) {
    float shininess = materialConstants.shiniess * 256.0f;
    float3 halfVec = normalize(lightVec + toEye);
    float roughness = (shininess + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), shininess) / 8.0f;
    float3 specularAlbedo = roughness * SchlickFresnel(materialConstants.fresnelR0, halfVec, lightVec);
    return (materialConstants.diffuseAlbedo.rgb + specularAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light light, float3 normal, float3 toEye) {
    float3 lightVec = -light.direction;
    float lambert = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = lambert * light.strength;
    return BlinnPhong(lightStrength, lightVec, normal, toEye);
}

float3 ComputePointLight(Light light, float3 position, float3 normal, float3 toEye) {
    float3 lightVec = light.position - position;
    float distance = length(lightVec);
    lightVec /= distance;
    distance *= light.attenuation;
    
    float lambert = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = lambert * light.strength / (distance * distance);
    
    return BlinnPhong(lightStrength, lightVec, normal, toEye);
}

float3 ComputeSpotLight(Light light, float3 position, float3 normal, float3 toEye) {
    float3 lightVec = light.position - position;
    float distance = length(lightVec);
    lightVec /= distance;
    distance *= light.attenuation;
    
    float lambert = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = lambert * light.strength / (distance * distance);
    lightStrength *= pow(max(dot(-lightVec, light.direction), 0.0f), light.spotPower);

    return BlinnPhong(lightStrength, lightVec, normal, toEye);
}