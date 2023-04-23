#include "../../include/hlsl/common.hlsl"

struct VertexIn {
    float3 posL : LocalPos;
};

struct VertexOut {
    float3 posL : LocalPos;
    float4 posH : SV_Position;
};

ConstantBuffer<PassConstants> passConstants : register(b0);

VertexOut main(VertexIn input) {
    float3 posW = input.posL + passConstants.eyePos.xyz;
    float4x4 viewProjMatrix = mul(passConstants.projMatrix, passConstants.viewMatrix);
    
    VertexOut output;
    output.posL = input.posL;
    output.posH = mul(viewProjMatrix, float4(posW, 1.0f)).xyww;
    return output;
}