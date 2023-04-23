struct Payload {
    float4 color;
};

RaytracingAccelerationStructure rs : register(t0);
RWTexture2D<float4> renderTarget : register(u1);

[shader("raygeneration")]
void main() {
    Payload myPayload = { float4(0.0f, 0.0f, 0.0f, 0.0f) };
    
    // 相机属性
    const float aspect = (float) DispatchRaysDimensions().x / (float) DispatchRaysDimensions().y;
    const float halfFovY = 0.25f * 3.1415962f;
    
    float3 nearPlane;
    nearPlane.z = 1.0f;
    nearPlane.y = nearPlane.z * tan(halfFovY) * 2.0f;
    nearPlane.x = nearPlane.y * aspect;
    
    // 计算光线的原点和方向
    float delta = nearPlane.x / (float) DispatchRaysDimensions().x;
    
    float3 center = nearPlane.z * float3(0.0f, 0.0f, 1.0f);
    float3 direction = float3(0.0f, 0.0f, -1.0f)
        + (-nearPlane.x / 2.0f + ((float) DispatchRaysIndex().x + 0.5f) * delta) * float3(1.0f, 0.0f, 0.0f)
        + (nearPlane.y / 2.0f - ((float) DispatchRaysIndex().y + 0.5f) * delta) * float3(0.0f, 1.0f, 0.0f);
    
    RayDesc rayDesc;
    rayDesc.Origin = float3(0.0f, 0.0f, 1.0f);
    rayDesc.Direction = direction;
    rayDesc.TMin = 0.0f;
    rayDesc.TMax = 1000.0f;
   
    TraceRay(rs, 0x0u, 0xffu, 0u, 1u, 0u, rayDesc, myPayload);
    
    renderTarget[DispatchRaysIndex().xy] = myPayload.color;
}