struct Attribute {
    float2 bary;
};

struct Payload {
    float4 color;
};

[shader("closesthit")]
void main(inout Payload output, in Attribute attribute) {
    output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
}