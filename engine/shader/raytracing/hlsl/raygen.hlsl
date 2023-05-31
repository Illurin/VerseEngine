#include "..\..\include\hlsl\raycommon.hlsl"

struct PassConstants {
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float4x4 invViewMatrix;
    float4x4 invProjMatrix;
    float tMin;
    float tMax;
};

RaytracingAccelerationStructure rs : register(t0);
RWTexture2D<float4> renderTarget : register(u1);

ConstantBuffer<PassConstants> passConstants : register(b0, space1);

[shader("raygeneration")]
void main() {
    
    // Calculate pixel information
    
    float2 pixelCenter = DispatchRaysIndex().xy + (float2) 0.5f;
    float2 screenPos = pixelCenter / DispatchRaysDimensions().xy;
    screenPos = screenPos * 2.0f - 1.0f;
    
    // Calculate ray information
    
    float3 rayOrigin    = mul(passConstants.invViewMatrix, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    float3 rayTarget    = mul(passConstants.invProjMatrix, float4(screenPos.x, screenPos.y, 1.0f, 1.0f)).xyz;
    float3 rayDirection = mul(passConstants.invViewMatrix, float4(normalize(rayTarget.xyz), 0.0f)).xyz;
    
    // Trace ray
    
    RayDesc rayDesc = {
        rayOrigin, passConstants.tMin, rayDirection, passConstants.tMax
    };
    Payload rayPayload;
    TraceRay(rs, 0x0u, 0xffu, 0u, 1u, 0u, rayDesc, rayPayload);
    
    renderTarget[DispatchRaysIndex().xy] = rayPayload.color;
}