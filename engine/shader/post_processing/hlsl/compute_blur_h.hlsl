#define groupSize 256
#define blurRadius 4
#define cacheSize (groupSize + 2 * blurRadius)

RWTexture2D<float4> shaderRead   : register(u0);
RWTexture2D<float4> renderTarget : register(u1);

groupshared float4 sharedMemory[cacheSize];

[numthreads(groupSize, 1, 1)]
void main(uint3 groupThreadId    : SV_GroupThreadID,
          uint3 dispatchThreadId : SV_DispatchThreadID) {
    uint2 imageSize;
    shaderRead.GetDimensions(imageSize.x, imageSize.y);
    
    const float weights[] = {
        0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216
    };
    
    if (groupThreadId.x < blurRadius) {
        uint clampedX = max(dispatchThreadId.x - blurRadius, 0);
        sharedMemory[groupThreadId.x] = shaderRead[uint2(clampedX, dispatchThreadId.y)];
    }
    else if (dispatchThreadId.x >= groupSize - blurRadius) {
        uint clampedX = min(dispatchThreadId.x + blurRadius, imageSize.x - 1);
        sharedMemory[groupThreadId.x + 2 * blurRadius] = shaderRead[uint2(clampedX, dispatchThreadId.y)];
    }
    
    sharedMemory[groupThreadId.x + blurRadius] = shaderRead[min(dispatchThreadId.xy, imageSize - 1)];
    
    GroupMemoryBarrierWithGroupSync();
    
    float4 blurColor = weights[0] * sharedMemory[groupThreadId.x + blurRadius];
    [unroll] for (int i = 1; i <= blurRadius; ++i) {
        blurColor += weights[i] * sharedMemory[groupThreadId.x + blurRadius - i];
        blurColor += weights[i] * sharedMemory[groupThreadId.x + blurRadius + i];
    }
    
    renderTarget[dispatchThreadId.xy] = blurColor;
}