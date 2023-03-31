struct CallData {
    float4 data;
};

[shader("callable")]
void main(inout CallData a) {
    a.data = float4(0.0f, 1.0f, 0.0f, 0.0f);
}