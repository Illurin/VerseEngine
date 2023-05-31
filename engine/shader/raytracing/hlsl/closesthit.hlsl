struct Attribute {
    float2 bary;
};

struct Payload {
    float4 color;
};

struct VertexConstants {
    float3 localPos;
    float3 normal;
    float3 tangent;
    float2 texCoord;
};

struct ObjectConstants {
    float4x4 worldMatrix;
    uint vertexOffset;
    uint indexOffset;
};

StructuredBuffer<VertexConstants> vertexConstantsArray : register(t0, space2);
StructuredBuffer<uint>            indices              : register(t1, space2);
StructuredBuffer<ObjectConstants> objectConstantsArray : register(t2, space2);

[shader("closesthit")]
void main(inout Payload output, in Attribute attribute) {
    ObjectConstants objectConstants = objectConstantsArray[InstanceID()];
    
    // Indices of the triangle
    uint3 triangleIndices = uint3(indices[objectConstants.indexOffset + PrimitiveIndex() * 3],
                                  indices[objectConstants.indexOffset + PrimitiveIndex() * 3 + 1],
                                  indices[objectConstants.indexOffset + PrimitiveIndex() * 3 + 2]);
    
    // Vertices of the triangle
    VertexConstants v0 = vertexConstantsArray[objectConstants.vertexOffset + triangleIndices.x];
    VertexConstants v1 = vertexConstantsArray[objectConstants.vertexOffset + triangleIndices.y];
    VertexConstants v2 = vertexConstantsArray[objectConstants.vertexOffset + triangleIndices.z];
    
    float3 worldPos0 = mul(objectConstants.worldMatrix, float4(v0.localPos, 1.0f)).xyz;
    float3 worldPos1 = mul(objectConstants.worldMatrix, float4(v0.localPos, 1.0f)).xyz;
    float3 worldPos2 = mul(objectConstants.worldMatrix, float4(v0.localPos, 1.0f)).xyz;
    
    float3 normal0 = mul(objectConstants.worldMatrix, float4(v0.normal, 0.0f)).xyz;
    float3 normal1 = mul(objectConstants.worldMatrix, float4(v0.normal, 0.0f)).xyz;
    float3 normal2 = mul(objectConstants.worldMatrix, float4(v0.normal, 0.0f)).xyz;
    
    float3 tangent0 = mul(objectConstants.worldMatrix, float4(v0.tangent, 0.0f)).xyz;
    float3 tangent1 = mul(objectConstants.worldMatrix, float4(v0.tangent, 0.0f)).xyz;
    float3 tangent2 = mul(objectConstants.worldMatrix, float4(v0.tangent, 0.0f)).xyz;
    
    // Interpolate
    const float3 bary = float3(1.0f - attribute.bary.x - attribute.bary.y, attribute.bary.x, attribute.bary.y);
    
    float3 worldPos = bary.x * worldPos0   + bary.y * worldPos1   + bary.z * worldPos2;
    float3 normal   = bary.x * normal0     + bary.y * normal1     + bary.z * normal2;
    float3 tangent  = bary.x * tangent0    + bary.y * tangent1    + bary.z * tangent2;
    float2 texCoord = bary.x * v0.texCoord + bary.y * v1.texCoord + bary.z * v2.texCoord;
    
    output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
}