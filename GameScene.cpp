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
	object = new Object();

	// --�J�����N���X�̃C���X�^���X����-- //
	camera = new Camera();

	// --���s�����N���X�̃C���X�^���X����-- //
	dirLight = DirectionalLight::GetInstance();
}

// --����������-- //
void GameScene::Initialize() {
	// --���̓N���X����������-- //
	input->Initialize();

	// --�e�N�X�`���̓ǂݍ���-- //
	marioGraph = texture->LoadTexture(L"Resources/mario.jpg");
	reimuGraph = texture->LoadTexture(L"Resources/reimu.png");

	// --�I�u�W�F�N�g����������-- //
	object->Initialize();

	// --�J�����̏���������-- //
	camera->eye = { 0.0f, 0.0f, -100.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->Initialize();

	// --���s��������������-- //
	dirLight->Initialize();
	dirLight->vec = { 1.0f, -1.0f, 1.0f };
}

// --�X�V����-- //
void GameScene::Update() {
	// --���̓N���X�X�V����-- //
	input->Update();

	// --�I�u�W�F�N�g��]����-- //
	object->rotation.y += input->PushKey(DIK_A) - input->PushKey(DIK_D);
	object->rotation.x += input->PushKey(DIK_W) - input->PushKey(DIK_S);

	// --�I�u�W�F�N�g�X�V����-- //
	object->Update(camera->matView, camera->matProjection);

	// --���s�����X�V����-- //
	dirLight->Update();
}

// --�`�揈��-- //
void GameScene::Draw() {

	// --���s�����`�揈��-- //
	dirLight->Draw();

	// --�I�u�W�F�N�g�`�揈��-- //
	object->DrawCube();

}