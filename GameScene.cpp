#include "GameScene.h"

#include "DrawCommSet.h"

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
GameScene::GameScene() : marioGraph(0), reimuGraph(0), angleX(0.0f), angleY(90.0f), forwardVec{},
cameraRotaSpeed(0.05f), playerSpeed(1.0f), length(10.0f), isActive(false) {
	// --入力クラスのインスタンス取得-- //
	input = Input::GetInstance();

	// --テクスチャクラスのインスタンス取得-- //
	texture = Texture::GetInstance();

	// --カメラクラスのインスタンス生成-- //
	camera = new Camera();

	// --プレイヤーオブジェクトのインスタンス生成-- //
	player = new Object();
}

// --初期化処理-- //
void GameScene::Initialize() {
	// --入力クラス初期化処理-- //
	input->Initialize();

	// --テクスチャの読み込み-- //
	marioGraph = texture->LoadTexture(L"Resources/mario.jpg");
	reimuGraph = texture->LoadTexture(L"Resources/reimu.png");
	haeGraph = texture->LoadTexture(L"Resources/texture.png");

	// --カメラの初期化処理-- //
	camera->eye = { 0.0f, 10.0f, 0.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->Initialize();

	// --プレイヤーオブジェクト初期化処理-- //
	player->position = { 0.0f, 10.0f, 0.0f };
	player->scale = { 1.0f, 2.0f, 1.0f };
	player->rotation = { 0.0f, 0.0f, 0.0f };
	player->Initialize();
}

// --更新処理-- //
void GameScene::Update() {
	// --入力クラス更新処理-- //
	input->Update();

	// --カメラの操作-- //
	{
		// --右ベクトル-- //
		Vector3 rightVec{};

		// --マウスの移動量で角度を変更
		angleX -= input->GetMouseVelosity().x * cameraRotaSpeed;// ← カメラの回転速度
		angleY += input->GetMouseVelosity().y * cameraRotaSpeed;// ← カメラの回転速度

		// --カメラの角度とプレイヤーを合わせる-- //
		player->rotation.y = angleX;

		// --上下の回転を制限する
		angleY = Util::Clamp(angleY, 180.0f, 0.0f);

		// --前方ベクトル-- //
		forwardVec.x = cosf(Util::Degree2Radian(angleX));
		forwardVec.y = cosf(Util::Degree2Radian(angleY));
		forwardVec.z = sinf(Util::Degree2Radian(angleX));
		forwardVec.normalize();// ←正規化

		// --右のベクトルを求める-- //
		rightVec = Vector3(forwardVec.x, 0.0f, forwardVec.z);
		rightVec = rightVec.cross(rightVec + Vector3(0.0f, 1.0f, 0.0f));
		rightVec.normalize();// ←正規化

		// --前に進む-- //
		if (input->PushKey(DIK_W)) {
			camera->eye.x += forwardVec.x * playerSpeed;
			camera->eye.z += forwardVec.z * playerSpeed;
			player->position.x += forwardVec.x * playerSpeed;
			player->position.z += forwardVec.z * playerSpeed;
		}

		// --後ろに進む-- //
		if (input->PushKey(DIK_S)) {
			camera->eye.x -= forwardVec.x * playerSpeed;
			camera->eye.z -= forwardVec.z * playerSpeed;
			player->position.x -= forwardVec.x * playerSpeed;
			player->position.z -= forwardVec.z * playerSpeed;
		}

		// --左に進む-- //
		if (input->PushKey(DIK_A)) {
			camera->eye.x += rightVec.x * playerSpeed;
			camera->eye.z += rightVec.z * playerSpeed;
			player->position.x += rightVec.x * playerSpeed;
			player->position.z += rightVec.z * playerSpeed;
		}

		// --右に進む-- //
		if (input->PushKey(DIK_D)) {
			camera->eye.x -= rightVec.x * playerSpeed;
			camera->eye.z -= rightVec.z * playerSpeed;
			player->position.x -= rightVec.x * playerSpeed;
			player->position.z -= rightVec.z * playerSpeed;
		}

		// --ダッシュ処理-- //
		if (input->PushKey(DIK_LSHIFT)) playerSpeed = 1.7f;
		else playerSpeed = 1.0f;

		camera->eye.y += input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN);

		// --注視点を変更
		camera->target.x = camera->eye.x + cosf(Util::Degree2Radian(angleX)) * length;
		camera->target.y = camera->eye.y + cosf(Util::Degree2Radian(angleY)) * length;
		camera->target.z = camera->eye.z + sinf(Util::Degree2Radian(angleX)) * length;
	}

	// --プレイヤー更新処理-- //
	player->Update(camera->matView, camera->matProjection);

	// --カメラ更新処理-- //
	camera->Update();
}

// --描画処理-- //
void GameScene::Draw() {

	// --3D用の共通設定をコマンドリストに積む-- //
	DrawCommSet::DrawCommSet3D();

	// --2D用の共通設定をコマンドリストに積む-- //
	DrawCommSet::DrawCommSet2D();
}