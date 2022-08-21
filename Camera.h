#pragma once

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --便利関数-- //
#include "Util.h"

class Camera {
	/// --メンバ変数-- ///
public:
	// --透視投影行列の計算-- //
	XMMATRIX matProjection;

	// --垂直方向視野角-- //
	float fovAngleY;

	// --ビューポートのアスペクト比-- //
	float aspectRatio;

	// --深度限界（手前側）-- //
	float nearZ;

	// --深度限界（奥側）
	float farZ;

	// --ビュー変換行列-- //
	XMMATRIX matView;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

private:

	/// --メンバ変数END-- ///
	/// --------------- ///
	/// --メンバ関数-- ///
public:
	// --コンストラクタ-- //
	Camera();

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update();

private:

	/// --メンバ関数END-- ///
};