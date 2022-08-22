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

struct ConstBufferDataDirLight {
	XMFLOAT3 vec;
};

class DirectionalLight {
/// --メンバ変数-- ///
public:
	// --平原光源のベクトル-- //
	XMFLOAT3 vec;

private:
	// --インスタンス-- //
	static DirectionalLight* myInstance;

	// --DirectX3Dクラス-- //
	DXManager* dxMa;

	// --定数バッファ（ライトの座標用）
	ComPtr<ID3D12Resource> constBuffDirLight;

	// --定数バッファ用データ構造（平行光源のベクトル）
	ConstBufferDataDirLight* constMapDirLight;

/// --メンバ変数END-- ///
/// --------------- ///
/// メンバ関数-- ///
public:
	// --インスタンス読み込み-- //
	static DirectionalLight* GetInstance();

	// --インスタンス解放-- //
	void Relese();

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update();

	// --描画処理-- //
	void Draw();

private:
	// --コンストラクタ-- //
	DirectionalLight();

/// --メンバ関数END-- ///
};