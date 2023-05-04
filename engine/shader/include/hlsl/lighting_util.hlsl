struct MaterialConstants {
    float4 diffuseAlbedo;
    float3 fresnelR0;
    float roughness;
};

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

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, MaterialConstants mat) {
    float shininess = (1.0f - mat.roughness) * 256.0f;
    float3 halfVec = normalize(lightVec + toEye);
    float roughness = (shininess + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), shininess) / 8.0f;
    float3 specularAlbedo = roughness * SchlickFresnel(mat.fresnelR0, halfVec, lightVec);
    return (mat.diffuseAlbedo.rgb + specularAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light light, float3 normal, float3 toEye, MaterialConstants mat) {
    float3 lightVec = -light.direction;
    float lambert = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = lambert * light.strength;
    return max(BlinnPhong(lightStrength, lightVec, normal, toEye, mat), 0.0f);
}

float3 ComputePointLight(Light light, float3 position, float3 normal, float3 toEye, MaterialConstants mat) {
    float3 lightVec = light.position - position;
    float distance = length(lightVec);
    lightVec /= distance;
    distance *= light.attenuation;
    
    float lambert = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = lambert * light.strength / (distance * distance);
    
    return max(BlinnPhong(lightStrength, lightVec, normal, toEye, mat), 0.0f);
}

float3 ComputeSpotLight(Light light, float3 position, float3 normal, float3 toEye, MaterialConstants mat) {
    float3 lightVec = light.position - position;
    float distance = length(lightVec);
    lightVec /= distance;
    distance *= light.attenuation;
    
    float lambert = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = lambert * light.strength / (distance * distance);
    lightStrength *= pow(max(dot(-lightVec, light.direction), 0.0f), light.spotPower);

    return max(BlinnPhong(lightStrength, lightVec, normal, toEye, mat), 0.0f);
}

float3 GetWorldSpaceNormal(float3 sampledNormal, float3 unitNormalW, float3 tangentW) {
	float3 normalT = 2.0f * sampledNormal - 1.0f;
    
	float3 n = unitNormalW;
	float3 t = normalize(tangentW - dot(tangentW, n) * n);
	float3 b = cross(n, t);

	float3x3 tbnMatrix = float3x3(t, b, n);
    
    float3 normalW = mul(tbnMatrix, normalT);
    normalW.z = -normalW.z;

    return normalW;
}