#define groupSize 256
#define blurRadius 4
#define cacheSize (groupSize + 2 * blurRadius)

RWTexture2D<float4> shaderRead   : register(u0);
RWTexture2D<float4> renderTarget : register(u1);

groupshared float4 sharedMemory[cacheSize];

[numthreads(1, groupSize, 1)]
void main(uint3 groupThreadId    : SV_GroupThreadID,
          uint3 dispatchThreadId : SV_DispatchThreadID) {
    uint2 imageSize;
    shaderRead.GetDimensions(imageSize.x, imageSize.y);

    const float weights[] = {
        0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216
    };
    
    if (groupThreadId.y < blurRadius) {
        uint clampedY = max(dispatchThreadId.y - blurRadius, 0);
        sharedMemory[groupThreadId.y] = shaderRead[uint2(dispatchThreadId.x, clampedY)];
    }
    else if (dispatchThreadId.y >= groupSize - blurRadius) {
        uint clampedY = min(dispatchThreadId.y + blurRadius, imageSize.y - 1);
        sharedMemory[groupThreadId.y + 2 * blurRadius] = shaderRead[uint2(dispatchThreadId.x, clampedY)];
    }
    
    sharedMemory[groupThreadId.y + blurRadius] = shaderRead[min(dispatchThreadId.xy, imageSize - 1)];
    
    GroupMemoryBarrierWithGroupSync();
    
    float4 blurColor = weights[0] * sharedMemory[groupThreadId.y + blurRadius];
    for (int i = 1; i <= blurRadius; ++i) {
        blurColor += weights[i] * sharedMemory[groupThreadId.y + blurRadius - i];
        blurColor += weights[i] * sharedMemory[groupThreadId.y + blurRadius + i];
    }
    
    // renderTarget[dispatchThreadId.xy] = blurColor;
    renderTarget[dispatchThreadId.xy] = shaderRead[dispatchThreadId.xy];
}