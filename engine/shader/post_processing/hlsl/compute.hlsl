struct ComputeSpace {
    uint3 groupId          : SV_GroupID;
    uint3 groupThreadId    : SV_GroupThreadID;
    uint3 dispatchThreadId : SV_DispatchThreadID;
    uint  groupIndex       : SV_GroupIndex;
};

RWTexture2D<float4> shaderRead   : register(u0);
RWTexture2D<float4> renderTarget : register(u1);

//[numthreads(1, 1, 1)]
//void BlurHMain(ComputeSpace computeSpace) {
    
//}

//[numthreads(1, 1, 1)]
//void BlurVMain(ComputeSpace computeSpace) {
    
//}

[numthreads(1, 1, 1)]
void main() {
    
}