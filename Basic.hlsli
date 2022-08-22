// --定数バッファ用データ構造体（マテリアル）-- //
cbuffer ConstBufferDataMaterial : register(b0)
{
	// --色（RGBA）
	float4 color;
}

// --定数バッファ用データ構造（3D変換行列）-- //
cbuffer ConstBufferDataTransform : register(b1)
{
	// 3D変換行列
	matrix mat;
	matrix world;
	//float3 transform;
	//matrix view;
	//matrix projection;
}

// --定数バッファ用データ構造（平行光源のベクトル）
cbuffer ConstBufferDataDirLight : register(b2)
{
	// --ベクトル-- //
	float3 vec;
}

// --定数バッファ-- //
cbuffer ConstBufferDataPointLight : register(b3)
{
	// --光源座標-- //
	float3 ptLightPos;

	// --光源減衰パラメータ-- //
	float3 attenuation;
};

// --頂点シェーダーの出力構造体-- //
// --頂点シェーダーからピクセルシェーダーへのやり取りに使用する-- //
struct VSOutput
{
	// --システム用頂点座標
	float4 svpos : SV_POSITION;

	// --法線ベクトル
	//float3 normal : NORMAL;

	// --ワールド座標系normal-- //
	float3 norw : NORMAL;

	// --uv値
	float2 uv : TEXCOORD;

	// --ワールド座標系の座標
	float3 posw : POSITION;
};