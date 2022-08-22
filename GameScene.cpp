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
	floor = new Object();
	object[0] = new Object();
	object[1] = new Object();

	// --カメラクラスのインスタンス生成-- //
	camera = new Camera();

	// --平行光源クラスのインスタンス生成-- //
	dirLight = DirectionalLight::GetInstance();

	// --点光源クラスのインスタンス生成-- //
	pointLight = new PointLight();
}

// --初期化処理-- //
void GameScene::Initialize() {
	// --入力クラス初期化処理-- //
	input->Initialize();

	// --テクスチャの読み込み-- //
	marioGraph = texture->LoadTexture(L"Resources/mario.jpg");
	reimuGraph = texture->LoadTexture(L"Resources/reimu.png");

	// --オブジェクト初期化処理-- //
	floor->scale = { 100.0f, 0.01, 100.0f };
	floor->position = { 0.0f, -5.0f, 0.0f };
	floor->Initialize();
	object[0]->Initialize();
	object[1]->scale = { 0.3f, 0.3f, 0.3f };
	object[1]->Initialize();

	// --カメラの初期化処理-- //
	camera->eye = { 50.0f, 30.0f, -100.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->Initialize();

	// --平行光源初期化処理-- //
	dirLight->Initialize();
	dirLight->vec = { 1.0f, -1.0f, 1.0f };

	pointLight->Initialize();
	pointLight->ptLightPos = { -10.0f, 10.0f, -10.0f };
	pointLight->attenuation = { 5.0f, 0.0f, 2.0f };
}

// --更新処理-- //
void GameScene::Update() {
	// --入力クラス更新処理-- //
	input->Update();

	// --オブジェクト回転処理-- //
	object[0]->rotation.y += input->PushKey(DIK_A) - input->PushKey(DIK_D);
	object[0]->rotation.x += input->PushKey(DIK_W) - input->PushKey(DIK_S);

	pointLight->ptLightPos.x += input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT);
	pointLight->ptLightPos.z += input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN);

	object[1]->position = pointLight->ptLightPos;

	// --オブジェクト更新処理-- //
	floor->Update(camera->matView, camera->matProjection);
	object[0]->Update(camera->matView, camera->matProjection);
	object[1]->Update(camera->matView, camera->matProjection);

	// --平行光源更新処理-- //
	dirLight->Update();

	// --点光源更新処理-- //
	pointLight->Update();
}

// --描画処理-- //
void GameScene::Draw() {

	// --平行光源描画処理-- //
	dirLight->Draw();

	// --点光源描画処理-- //
	pointLight->Draw();

	// --オブジェクト描画処理-- //
	floor->DrawCube();
	object[0]->DrawCube(marioGraph);
	object[1]->DrawCube();

}