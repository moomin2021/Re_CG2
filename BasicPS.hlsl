cbuffer ConstBufferMaterial : register(b0)
{
	// �F�iRGBA�j
	float4 color;
}

float4 main() : SV_TARGET
{
	return color;
}