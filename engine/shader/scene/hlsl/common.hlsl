struct PassConstants {
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float4 eyePos;
    float3 ambientLight;
};

ConstantBuffer<PassConstants> passConstants : register(b0, space2);