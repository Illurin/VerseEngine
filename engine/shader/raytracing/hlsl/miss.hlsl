struct Payload {
    float4 color;
};

[shader("miss")]
void main(inout Payload a) {
    a.color = float4(0.0f, 0.0f, 0.0f, 0.0f);
}