#define groupSize 16

RWTexture2D<float4> shaderRead   : register(u0);
RWTexture2D<float4> renderTarget : register(u1);

[numthreads(groupSize, groupSize, 1)]
void main(uint3 groupThreadId    : SV_GroupThreadID,
          uint3 dispatchThreadId : SV_DispatchThreadID) {
    const float3 grayWeights = float3(0.2126, 0.7152, 0.0722);
    
    float4 fragColor = shaderRead[dispatchThreadId.xy];
    float luminance = dot(fragColor.rgb, grayWeights);
    
    float3 extractedColor = (float3) 0.0f;
    if (luminance > 1.0f) {
        extractedColor = fragColor.rgb;
    }
    
    renderTarget[dispatchThreadId.xy] = float4(extractedColor, 1.0f);
}