#include "Camera.h"

// --�R���X�g���N�^-- //
Camera::Camera() : matProjection{}, fovAngleY(Util::Degree2Radian(45.0f)),
aspectRatio((float)16 / 9), nearZ(0.1f), farZ(1000.0f),
matView{}, eye{}, target{}, up{}
{

}

// --����������-- //
void Camera::Initialize() {
	// --�������e�s��̌v�Z-- //
	matProjection = XMMatrixPerspectiveFovLH(
		fovAngleY,// -----------> �㉺��p45�x
		aspectRatio,// -> �A�X�y�N�g��i��ʉ���/��ʏc���j
		nearZ, farZ// ------------------------> �O�[�A���[
	);

	// --�r���[�ϊ��s��-- //
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

// --�X�V����-- //
void Camera::Update() {
	// --�������e�s��̌v�Z-- //
	matProjection = XMMatrixPerspectiveFovLH(
		fovAngleY,// -----------> �㉺��p45�x
		aspectRatio,// -> �A�X�y�N�g��i��ʉ���/��ʏc���j
		nearZ, farZ// ------------------------> �O�[�A���[
	);

	// --�r���[�ϊ��s��-- //
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

