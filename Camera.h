#pragma once

// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

// --�֗��֐�-- //
#include "Util.h"

class Camera {
	/// --�����o�ϐ�-- ///
public:
	// --�������e�s��̌v�Z-- //
	XMMATRIX matProjection;

	// --������������p-- //
	float fovAngleY;

	// --�r���[�|�[�g�̃A�X�y�N�g��-- //
	float aspectRatio;

	// --�[�x���E�i��O���j-- //
	float nearZ;

	// --�[�x���E�i�����j
	float farZ;

	// --�r���[�ϊ��s��-- //
	XMMATRIX matView;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

private:

	/// --�����o�ϐ�END-- ///
	/// --------------- ///
	/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	Camera();

	// --����������-- //
	void Initialize();

	// --�X�V����-- //
	void Update();

private:

	/// --�����o�֐�END-- ///
};