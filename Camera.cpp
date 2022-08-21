#include "Camera.h"

// --コンストラクタ-- //
Camera::Camera() : matProjection{}, fovAngleY(Util::Degree2Radian(45.0f)),
aspectRatio((float)16 / 9), nearZ(0.1f), farZ(1000.0f),
matView{}, eye{}, target{}, up{}
{

}

// --初期化処理-- //
void Camera::Initialize() {
	// --透視投影行列の計算-- //
	matProjection = XMMatrixPerspectiveFovLH(
		fovAngleY,// -----------> 上下画角45度
		aspectRatio,// -> アスペクト比（画面横幅/画面縦幅）
		nearZ, farZ// ------------------------> 前端、奥端
	);

	// --ビュー変換行列-- //
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

// --更新処理-- //
void Camera::Update() {
	// --透視投影行列の計算-- //
	matProjection = XMMatrixPerspectiveFovLH(
		fovAngleY,// -----------> 上下画角45度
		aspectRatio,// -> アスペクト比（画面横幅/画面縦幅）
		nearZ, farZ// ------------------------> 前端、奥端
	);

	// --ビュー変換行列-- //
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

