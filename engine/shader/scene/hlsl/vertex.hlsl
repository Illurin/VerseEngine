#include "common.hlsl"

struct VertexIn {
    float3 posL : LocalPos;
    float3 normal : Normal;
    float2 texCoord : TexCoord;
};

struct VertexOut {
    float3 normal : Normal;
	float2 texCoord : TexCoord;
    float3 posW : WorldPos;
	float4 posH : SV_Position;
};

struct ObjectConstants {
    float4x4 worldMatrix;
};

ConstantBuffer<ObjectConstants> objectConstants : register(b0, space0);

VertexOut main(VertexIn input) {
    float3 posW = mul(objectConstants.worldMatrix, float4(input.posL, 1.0f)).xyz;
    float4x4 viewProjMatrix = mul(passConstants.projMatrx, passConstants.viewMatrx);
    
	VertexOut output;
    output.posH = mul(viewProjMatrix, float4(posW, 1.0f));
    output.normal = input.normal;
	output.texCoord = input.texCoord;
    output.posW = posW;
	return output;
}