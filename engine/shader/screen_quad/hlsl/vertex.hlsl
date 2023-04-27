struct VertexOut {
    float2 texCoord : TexCoord;
    float4 position : SV_Position;
};

VertexOut main(uint vertexId : SV_VertexID) {
    VertexOut output;
    
    switch (vertexId) {
    case 0:
        output.position = float4(-1.0, -1.0f, 0.0f, 1.0f);
        output.texCoord = float2(0.0, 1.0f);
        break;
    case 1:
        output.position = float4(1.0, -1.0f, 0.0f, 1.0f);
        output.texCoord = float2(1.0, 1.0f);
        break;
    case 2:
        output.position = float4(-1.0, 1.0f, 0.0f, 1.0f);
        output.texCoord = float2(0.0, 0.0f);
        break;
    case 3:
        output.position = float4(1.0, 1.0f, 0.0f, 1.0f);
        output.texCoord = float2(1.0, 0.0f);
        break;
    }
    
    return output;
}