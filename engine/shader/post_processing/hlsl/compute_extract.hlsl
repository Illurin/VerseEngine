#define groupSize 16

RWTexture2D<float4> shaderRead   : register(u0);
RWTexture2D<float4> renderTarget : register(u1);

[numthreads(groupSize, groupSize, 1)]
void main(uint3 groupThreadId    : SV_GroupThreadID,
          uint3 dispatchThreadId : SV_DispatchThreadID) {
    const float3 luminanceWeights = float3(0.299, 0.587, 0.114);
    
    float3 fragColor = shaderRead[dispatchThreadId.xy].rgb;
    float luminance = dot(fragColor, luminanceWeights);
    
    float bloomThreshold = 1.0f;
    
    luminance = saturate(luminance - bloomThreshold);
    renderTarget[dispatchThreadId.xy] = float4(luminance * fragColor, 1.0f);
}