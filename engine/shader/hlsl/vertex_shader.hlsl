struct VertexIn {
	float3 posW : WorldPos;
	float3 color : Color;
	uint vertexId : SV_VertexID;
};

struct VertexOut {
	float3 color : Color;
	float4 posH : SV_Position;
};

VertexOut main(VertexIn input) {
	VertexOut output;
	output.posH = float4(input.posW, 1.0f);
	output.color = input.color;

	return output;
}