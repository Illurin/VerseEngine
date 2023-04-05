#include "common.hlsl"

struct VertexIn {
	float3 posW : WorldPos;
	float3 normal : Normal;
	float2 texCoord : TexCoord;
};

struct VertexOut {
	float2 texCoord : TexCoord;
	float4 posH : SV_Position;
};

struct PassConstants {
	float4x4 worldMatrix;
};

ConstantBuffer<PassConstants> passConstants : register(b0, space0);

VertexOut main(VertexIn input) {
	VertexOut output;

    output.posH = float4(input.posW, 1.0f);
	output.texCoord = input.texCoord;

	return output;
}