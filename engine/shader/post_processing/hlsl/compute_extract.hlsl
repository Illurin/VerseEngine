#define groupSize 32
#define cacheSize (groupSize * groupSize)

RWTexture2D<float4> shaderRead   : register(u0);
RWTexture2D<float4> renderTarget : register(u1);

groupshared float4 sharedMemory[cacheSize];

[numthreads(groupSize, groupSize, 1)]
void main(uint3 groupThreadId    : SV_GroupThreadID,
          uint3 dispatchThreadId : SV_DispatchThreadID) {
    // renderTarget[dispatchThreadId.xy] = shaderRead[dispatchThreadId.xy];
    renderTarget[dispatchThreadId.xy] = float4(1.0f, 1.0f, 0.0f, 1.0f);
}