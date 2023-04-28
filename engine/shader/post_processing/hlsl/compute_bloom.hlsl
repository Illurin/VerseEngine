#define groupSize 16

struct PostProcessingProperties {
    float exposure;
    float gamma;
};

RWTexture2D<float4> source0      : register(u0);
RWTexture2D<float4> source1      : register(u1);
RWTexture2D<float4> renderTarget : register(u2);

[numthreads(groupSize, groupSize, 1)]
void main(uint3 dispatchThreadId : SV_DispatchThreadID) {
    PostProcessingProperties properties = { 0.8f, 2.2f };

    float3 combinedColor = source0[dispatchThreadId.xy].rgb + source1[dispatchThreadId.xy].rgb;
    combinedColor = (float3) 1.0f - exp(-properties.exposure * combinedColor);
    combinedColor = pow(combinedColor, (float3) (1.0f / properties.gamma));
    
    renderTarget[dispatchThreadId.xy] = float4(combinedColor, 1.0f);
}