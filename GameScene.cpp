#include "GameScene.h"

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
GameScene::GameScene() : marioGraph(0), reimuGraph(0) {
	// --���̓N���X�̃C���X�^���X�擾-- //
	input = Input::GetInstance();

	// --�e�N�X�`���N���X�̃C���X�^���X�擾-- //
	texture = Texture::GetInstance();

	// --�I�u�W�F�N�g�C���X�^���X����-- //
	floor = new Object();
	object[0] = new Object();
	object[1] = new Object();

	// --�J�����N���X�̃C���X�^���X����-- //
	camera = new Camera();

	// --���s�����N���X�̃C���X�^���X����-- //
	dirLight = DirectionalLight::GetInstance();

	// --�_�����N���X�̃C���X�^���X����-- //
	pointLight = new PointLight();
}

// --����������-- //
void GameScene::Initialize() {
	// --���̓N���X����������-- //
	input->Initialize();

	// --�e�N�X�`���̓ǂݍ���-- //
	marioGraph = texture->LoadTexture(L"Resources/mario.jpg");
	reimuGraph = texture->LoadTexture(L"Resources/reimu.png");

	// --�I�u�W�F�N�g����������-- //
	floor->scale = { 100.0f, 0.01, 100.0f };
	floor->position = { 0.0f, -5.0f, 0.0f };
	floor->Initialize();
	object[0]->Initialize();
	object[1]->scale = { 0.3f, 0.3f, 0.3f };
	object[1]->Initialize();

	// --�J�����̏���������-- //
	camera->eye = { 50.0f, 30.0f, -100.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->Initialize();

	// --���s��������������-- //
	dirLight->Initialize();
	dirLight->vec = { 1.0f, -1.0f, 1.0f };

	pointLight->Initialize();
	pointLight->ptLightPos = { -10.0f, 10.0f, -10.0f };
	pointLight->attenuation = { 5.0f, 0.0f, 2.0f };
}

// --�X�V����-- //
void GameScene::Update() {
	// --���̓N���X�X�V����-- //
	input->Update();

	// --�I�u�W�F�N�g��]����-- //
	object[0]->rotation.y += input->PushKey(DIK_A) - input->PushKey(DIK_D);
	object[0]->rotation.x += input->PushKey(DIK_W) - input->PushKey(DIK_S);

	pointLight->ptLightPos.x += input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT);
	pointLight->ptLightPos.z += input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN);

	object[1]->position = pointLight->ptLightPos;

	// --�I�u�W�F�N�g�X�V����-- //
	floor->Update(camera->matView, camera->matProjection);
	object[0]->Update(camera->matView, camera->matProjection);
	object[1]->Update(camera->matView, camera->matProjection);

	// --���s�����X�V����-- //
	dirLight->Update();

	// --�_�����X�V����-- //
	pointLight->Update();
}

// --�`�揈��-- //
void GameScene::Draw() {

	// --���s�����`�揈��-- //
	dirLight->Draw();

	// --�_�����`�揈��-- //
	pointLight->Draw();

	// --�I�u�W�F�N�g�`�揈��-- //
	floor->DrawCube();
	object[0]->DrawCube(marioGraph);
	object[1]->DrawCube();

}