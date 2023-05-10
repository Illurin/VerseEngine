#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS       100
#define MAX_SPOT_LIGHTS        1
#define MAX_LIGHT_COUNT        (MAX_DIRECTIONAL_LIGHTS + MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS)
                                
struct LightConstants {
    float3 color;
    float intensity;
    float3 direction;
    float attenuation;
    float3 position;
    float spotPower;
};

struct PassConstants {
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float4 eyePos;
    float4 ambientLight;
    LightConstants lights[MAX_LIGHT_COUNT];
};

float3 GetWorldSpaceNormal(float3 sampledNormal, float3 unitNormalW, float3 tangentW) {
	float3 normalT = 2.0f * sampledNormal - 1.0f;
    
	float3 n = unitNormalW;
	float3 t = normalize(tangentW - dot(tangentW, n) * n);
	float3 b = cross(t, n);

	float3x3 tbnMatrix = float3x3(t, b, n);
    
    return mul(tbnMatrix, normalT);
}