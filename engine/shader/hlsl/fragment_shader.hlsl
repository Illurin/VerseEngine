struct FragmentIn {
	float3 color : Color;
};

float4 main(FragmentIn input) : SV_Target0 {
	return float4(input.color, 1.0f);
}