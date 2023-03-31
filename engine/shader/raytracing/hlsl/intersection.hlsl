struct Attribute {
    float2 bary;
};

[shader("intersection")]
void main() {
    Attribute myHitAttribute = { float2(0.0f, 0.0f) };
    ReportHit(0.0f, 0, myHitAttribute);
}