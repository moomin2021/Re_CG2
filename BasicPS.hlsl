cbuffer ConstBufferMaterial : register(b0)
{
	// êFÅiRGBAÅj
	float4 color;
}

float4 main() : SV_TARGET
{
	return color;
}