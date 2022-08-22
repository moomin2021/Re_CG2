#pragma once

// --���̓N���X-- //
#include "Input.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --�I�u�W�F�N�g�N���X-- //
#include "Object3D.h"

// --�J�����N���X-- //
#include "Camera.h"

// --���s�����N���X-- //
#include "DirectionalLight.h"

// --�_�����N���X-- //
#include "PointLight.h"

// --�x�N�^�[-- //
#include <vector>

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

	// --���s�����N���X-- //
	DirectionalLight* dirLight;

	// --�_�����N���X-- //
	PointLight* pointLight;

	// --�e�N�X�`���n���h���ۑ��p�ϐ�-- //
	int marioGraph;
	int reimuGraph;

	// --�I�u�W�F�N�g�N���X-- //
	Object* floor;
	Object* object[2];

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
