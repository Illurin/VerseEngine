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