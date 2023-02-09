struct VertexIn {
	float2 position : Position;
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

	output.posH = mul(passConstants.worldMatrix, float4(input.position, 0.0f, 1.0f));
	output.texCoord = input.texCoord;

	return output;
}