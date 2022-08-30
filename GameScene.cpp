#include "GameScene.h"

#include "DrawCommSet.h"

// --�C���X�^���X��NULL����-- //
GameScene* GameScene::myInstance = nullptr;

// --�C���X�^���X�ǂݍ���-- //
GameScene* GameScene::GetInstance() {
	// --�C���X�^���X�����������琶������-- //
	if (myInstance == nullptr) myInstance = new GameScene();

	// --�C���X�^���X��Ԃ�-- //
	return myInstance;
}

// --�C���X�^���X���-- //
void GameScene::Relese() {
	// --�C���X�^���X�����������牽�������ɏI������-- //
	if (myInstance == nullptr) return;

	// --�C���X�^���X���-- //
	delete myInstance;
	myInstance = nullptr;
}

// --�R���X�g���N�^-- //
GameScene::GameScene() : marioGraph(0), reimuGraph(0), angleX(0.0f), angleY(90.0f), forwardVec{},
cameraRotaSpeed(0.05f), playerSpeed(1.0f), length(10.0f) {
	// --���̓N���X�̃C���X�^���X�擾-- //
	input = Input::GetInstance();

	// --�e�N�X�`���N���X�̃C���X�^���X�擾-- //
	texture = Texture::GetInstance();

	// --�J�����N���X�̃C���X�^���X����-- //
	camera = new Camera();

	// --�_�����N���X�̃C���X�^���X����-- //
	pointLight = new PointLight();

	// --�v���C���[�I�u�W�F�N�g�̃C���X�^���X����-- //
	player = new Object();
}

// --����������-- //
void GameScene::Initialize() {
	// --���̓N���X����������-- //
	input->Initialize();

	// --�e�N�X�`���̓ǂݍ���-- //
	marioGraph = texture->LoadTexture(L"Resources/mario.jpg");
	reimuGraph = texture->LoadTexture(L"Resources/reimu.png");

	// --�J�����̏���������-- //
	camera->eye = { 0.0f, 10.0f, 0.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->Initialize();

	// --�_����-- //
	pointLight->Initialize();
	pointLight->ptLightPos = { 0.0f, 10.0f, 0.0f };
	pointLight->attenuation = { 100.0f, 0.0f, 2.0f };

	// --�v���C���[�I�u�W�F�N�g����������-- //
	player->position = { 0.0f, 10.0f, 0.0f };
	player->scale = { 1.0f, 2.0f, 1.0f };
	player->rotation = { 0.0f, 0.0f, 0.0f };
	player->Initialize();

	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 10; j++) {
			floor[i][j] = new Object();
			floor[i][j]->position = { 100.0f * i - 450.0f, 0.0f, 100.0f * j - 450.0f };
			floor[i][j]->scale = { 10.0f, 0.05f, 10.0f };
			if (i % 2 == 0) {
				if (j % 2 == 1) floor[i][j]->color = { 0.1f, 0.1f, 0.1f, 1.0f };
			}
			else {
				if (j % 2 == 0) floor[i][j]->color = { 0.1f, 0.1f, 0.1f, 1.0f };
			}
			floor[i][j]->Initialize();
		}
	}
}

// --�X�V����-- //
void GameScene::Update() {
	// --���̓N���X�X�V����-- //
	input->Update();

	// --�J�����̑���-- //
	{
		// --�E�x�N�g��-- //
		Vector3 rightVec{};

		// --�}�E�X�̈ړ��ʂŊp�x��ύX
		angleX -= input->GetMouseVelosity().x * cameraRotaSpeed;// �� �J�����̉�]���x
		angleY += input->GetMouseVelosity().y * cameraRotaSpeed;// �� �J�����̉�]���x

		// --�J�����̊p�x�ƃv���C���[�����킹��-- //
		player->rotation.y = angleX;

		// --�㉺�̉�]�𐧌�����
		angleY = Util::Clamp(angleY, 180.0f, 0.0f);

		// --�O���x�N�g��-- //
		forwardVec.x = cosf(Util::Degree2Radian(angleX));
		forwardVec.y = cosf(Util::Degree2Radian(angleY));
		forwardVec.z = sinf(Util::Degree2Radian(angleX));
		forwardVec.normalize();// �����K��

		// --�E�̃x�N�g�������߂�-- //
		rightVec = Vector3(forwardVec.x, 0.0f, forwardVec.z);
		rightVec = rightVec.cross(rightVec + Vector3(0.0f, 1.0f, 0.0f));
		rightVec.normalize();// �����K��

		// --�O�ɐi��-- //
		if (input->PushKey(DIK_W)) {
			camera->eye.x += forwardVec.x * playerSpeed;
			camera->eye.z += forwardVec.z * playerSpeed;
			player->position.x += forwardVec.x * playerSpeed;
			player->position.z += forwardVec.z * playerSpeed;
		}

		// --���ɐi��-- //
		if (input->PushKey(DIK_S)) {
			camera->eye.x -= forwardVec.x * playerSpeed;
			camera->eye.z -= forwardVec.z * playerSpeed;
			player->position.x -= forwardVec.x * playerSpeed;
			player->position.z -= forwardVec.z * playerSpeed;
		}

		// --���ɐi��-- //
		if (input->PushKey(DIK_A)) {
			camera->eye.x += rightVec.x * playerSpeed;
			camera->eye.z += rightVec.z * playerSpeed;
			player->position.x += rightVec.x * playerSpeed;
			player->position.z += rightVec.z * playerSpeed;
		}

		// --�E�ɐi��-- //
		if (input->PushKey(DIK_D)) {
			camera->eye.x -= rightVec.x * playerSpeed;
			camera->eye.z -= rightVec.z * playerSpeed;
			player->position.x -= rightVec.x * playerSpeed;
			player->position.z -= rightVec.z * playerSpeed;
		}

		// --�_�b�V������-- //
		if (input->PushKey(DIK_LSHIFT)) playerSpeed = 1.7f;
		else playerSpeed = 1.0f;

		//camera->eye.y += input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN);
		pointLight->ptLightPos.x += input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT);
		pointLight->ptLightPos.z += input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN);

		// --�����_��ύX
		camera->target.x = camera->eye.x + cosf(Util::Degree2Radian(angleX)) * length;
		camera->target.y = camera->eye.y + cosf(Util::Degree2Radian(angleY)) * length;
		camera->target.z = camera->eye.z + sinf(Util::Degree2Radian(angleX)) * length;
	}

	// --�v���C���[�X�V����-- //
	player->Update(camera->matView, camera->matProjection);

	// --���I�u�W�F�N�g�X�V����-- //
	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 10; j++) {
			floor[i][j]->Update(camera->matView, camera->matProjection);
		}
	}

	pointLight->ptLightPos = player->position;

	// --�J�����X�V����-- //
	camera->Update();

	// --�_�����X�V����-- //
	pointLight->Update();
}

// --�`�揈��-- //
void GameScene::Draw() {

	// --3D�p�̋��ʐݒ���R�}���h���X�g�ɐς�-- //
	DrawCommSet::DrawCommSet3D();

	// --�_�����`�揈��-- //
	pointLight->Draw();

	// --�v���C���[�`�揈��-- //
	//player->DrawCube();

	// --���I�u�W�F�N�g�`��-- //
	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 10; j++) {
			floor[i][j]->DrawCube();
		}
	}
}