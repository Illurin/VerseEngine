struct VertexIn {
	uint vertexId : SV_VertexID;
};

struct VertexOut {
	float4 posH : SV_Position;
};

VertexOut main(VertexIn input) {
	VertexOut output;

	if (input.vertexId == 0) {
		output.posH = float4(0.0f, 0.5f, 0.0f, 1.0f);
	}
	else if (input.vertexId == 1) {
		output.posH = float4(-0.5f, -0.5f, 0.0f, 1.0f);
	}
	else if (input.vertexId == 2) {
		output.posH = float4(0.5f, -0.5f, 0.0f, 1.0f);
	}

	return output;
}