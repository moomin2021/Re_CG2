#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --DirectX3Dクラス-- //
#include "DXManager.h"

// --定数バッファ-- //
struct ConstBufferDataPointLight {
	// --光源座標-- //
	XMFLOAT3 ptLightPos;

	// --光源減衰パラメータ-- //
	XMFLOAT3 attenuation;
};

class PointLight {
/// --メンバ変数-- ///
public:
	// --光源座標-- //
	XMFLOAT3 ptLightPos;

	// --光源減衰パラメーター-- //
	XMFLOAT3 attenuation;

private:
	// --DirectX3Dクラス-- //
	DXManager* dxMa;

	// --定数バッファ（点光源用）
	ComPtr<ID3D12Resource> constBuffPointLight;

	// --定数バッファ用データ構造（平行光源のベクトル）
	ConstBufferDataPointLight* constMapPointLight;

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --コンストラクタ-- //
	PointLight();

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update();

	// --描画処理-- //
	void Draw();

private:

/// --メンバ関数END-- ///
};