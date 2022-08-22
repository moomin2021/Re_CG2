#include "GameScene.h"

// --インスタンスにNULLを代入-- //
GameScene* GameScene::myInstance = nullptr;

// --インスタンス読み込み-- //
GameScene* GameScene::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance == nullptr) myInstance = new GameScene();

	// --インスタンスを返す-- //
	return myInstance;
}

// --インスタンス解放-- //
void GameScene::Relese() {
	// --インスタンスが無かったら何もせずに終了する-- //
	if (myInstance == nullptr) return;

	// --インスタンス解放-- //
	delete myInstance;
	myInstance = nullptr;
}

// --コンストラクタ-- //
GameScene::GameScene() : marioGraph(0), reimuGraph(0) {
	// --入力クラスのインスタンス取得-- //
	input = Input::GetInstance();

	// --テクスチャクラスのインスタンス取得-- //
	texture = Texture::GetInstance();

	// --オブジェクトインスタンス生成-- //
	object = new Object();

	// --カメラクラスのインスタンス生成-- //
	camera = new Camera();

	// --平行光源クラスのインスタンス生成-- //
	dirLight = DirectionalLight::GetInstance();
}

// --初期化処理-- //
void GameScene::Initialize() {
	// --入力クラス初期化処理-- //
	input->Initialize();

	// --テクスチャの読み込み-- //
	marioGraph = texture->LoadTexture(L"Resources/mario.jpg");
	reimuGraph = texture->LoadTexture(L"Resources/reimu.png");

	// --オブジェクト初期化処理-- //
	object->Initialize();

	// --カメラの初期化処理-- //
	camera->eye = { 0.0f, 0.0f, -100.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->Initialize();

	// --平行光源初期化処理-- //
	dirLight->Initialize();
	dirLight->vec = { 1.0f, -1.0f, 1.0f };
}

// --更新処理-- //
void GameScene::Update() {
	// --入力クラス更新処理-- //
	input->Update();

	// --オブジェクト回転処理-- //
	object->rotation.y += input->PushKey(DIK_A) - input->PushKey(DIK_D);
	object->rotation.x += input->PushKey(DIK_W) - input->PushKey(DIK_S);

	// --オブジェクト更新処理-- //
	object->Update(camera->matView, camera->matProjection);

	// --平行光源更新処理-- //
	dirLight->Update();
}

// --描画処理-- //
void GameScene::Draw() {

	// --平行光源描画処理-- //
	dirLight->Draw();

	// --オブジェクト描画処理-- //
	object->DrawCube();

}