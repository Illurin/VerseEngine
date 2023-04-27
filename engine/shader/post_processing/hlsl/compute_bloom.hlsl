#define groupSize 32
#define cacheSize (groupSize * groupSize)

RWTexture2D<float4> source0      : register(u0);
RWTexture2D<float4> source1      : register(u1);
RWTexture2D<float4> renderTarget : register(u2);

groupshared float4 sharedMemory[cacheSize];

[numthreads(groupSize, groupSize, 1)]
void main(uint3 dispatchThreadId : SV_DispatchThreadID) {
    renderTarget[dispatchThreadId.xy] = source0[dispatchThreadId.xy];
}