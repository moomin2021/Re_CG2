#pragma once

// --���̓N���X-- //
#include "Input.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --�I�u�W�F�N�g�N���X-- //
#include "Object3D.h"

// --�J�����N���X-- //
#include "Camera.h"

// --�_�����N���X-- //
#include "PointLight.h"

// --�x�N�^�[-- //
#include <vector>

// --Vector3�N���X-- //
#include "Vector3.h"

// --�X�v���C�g�N���X-- //
#include "Sprite.h"

class GameScene {
/// --�����o�ϐ�-- ///
public:

private:
	// --�C���X�^���X-- //
	static GameScene* myInstance;

	// --���̓N���X-- //
	Input* input;

	// --�e�N�X�`���N���X-- //
	Texture* texture;

	// --�J�����N���X-- //
	Camera* camera;

	// --�_�����N���X-- //
	PointLight* pointLight;

	// --�e�N�X�`���n���h���ۑ��p�ϐ�-- //
	int marioGraph;
	int reimuGraph;
	int haeGraph;

	// --�v���C���[�I�u�W�F�N�g-- //
	Object* player;

	// --�J�����̊p�x-- //
	float angleX, angleY;

	// --�O���x�N�g��-- //
	Vector3 forwardVec;

	// --�J�����̉�]���x-- //
	float cameraRotaSpeed;

	// --�v���C���[�̈ړ����x-- //
	float playerSpeed;

	// --�J�����ƒ����_�̋���-- //
	float length;

	// --���I�u�W�F�N�g-- //
	Object * floor[10][10];

	// --�X�v���C�g-- //
	Sprite* sprite[2];

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�ǂݍ���-- //
	static GameScene* GetInstance();

	// --�C���X�^���X���-- //
	void Relese();

	// --������-- //
	void Initialize();

	// --�X�V����-- //
	void Update();

	// --�`�揈��-- //
	void Draw();

private:
	// --�R���X�g���N�^-- //
	GameScene();

/// --�����o�֐�END-- ///
};
