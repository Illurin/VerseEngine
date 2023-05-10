#include "..\..\include\hlsl\common.hlsl"
#include "..\..\include\hlsl\lighting_util.hlsl"

struct FragmentIn {
    float3 normal   : Normal;
    float3 tangent  : Tangent;
    float2 texCoord : TexCoord;
    float3 posW     : WorldPos;
};

struct FragmentOut {
    float4 normal        : SV_Target0;
    float4 worldPos      : SV_Target1;
    float4 diffuseAlbedo : SV_Target2;
    float4 material      : SV_Target3;
};

ConstantBuffer<MaterialConstants> materialConstants : register(b0, space1);
Texture2D                         diffuseMap        : register(t1, space1);
SamplerState                      diffuseMapSampler : register(s2, space1);
Texture2D                         normalMap         : register(t3, space1);
SamplerState                      normalMapSampler  : register(s4, space1);

FragmentOut main(FragmentIn input) {
    FragmentOut output;
    
    output.diffuseAlbedo = materialConstants.diffuseAlbedo * diffuseMap.Sample(diffuseMapSampler, input.texCoord);
    
    if (output.diffuseAlbedo.a <= 0.0f) discard;
    
    output.normal = float4(normalize(input.normal), 0.0f);
    output.worldPos = float4(input.posW, 1.0f);
    output.material = float4(materialConstants.fresnelR0, materialConstants.roughness);
    
    return output;
}