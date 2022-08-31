#pragma once

// --入力クラス-- //
#include "Input.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --オブジェクトクラス-- //
#include "Object3D.h"

// --カメラクラス-- //
#include "Camera.h"

// --点光源クラス-- //
#include "PointLight.h"

// --ベクター-- //
#include <vector>

// --Vector3クラス-- //
#include "Vector3.h"

// --スプライトクラス-- //
#include "Sprite.h"

class GameScene {
/// --メンバ変数-- ///
public:

private:
	// --インスタンス-- //
	static GameScene* myInstance;

	// --入力クラス-- //
	Input* input;

	// --テクスチャクラス-- //
	Texture* texture;

	// --カメラクラス-- //
	Camera* camera;

	// --点光源クラス-- //
	PointLight* pointLight;

	// --テクスチャハンドル保存用変数-- //
	int marioGraph;
	int reimuGraph;
	int haeGraph;

	// --プレイヤーオブジェクト-- //
	Object* player;

	// --カメラの角度-- //
	float angleX, angleY;

	// --前方ベクトル-- //
	Vector3 forwardVec;

	// --カメラの回転速度-- //
	float cameraRotaSpeed;

	// --プレイヤーの移動速度-- //
	float playerSpeed;

	// --カメラと注視点の距離-- //
	float length;

	// --床オブジェクト-- //
	Object * floor[10][10];

	// --スプライト-- //
	Sprite* sprite[2];

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --インスタンス読み込み-- //
	static GameScene* GetInstance();

	// --インスタンス解放-- //
	void Relese();

	// --初期化-- //
	void Initialize();

	// --更新処理-- //
	void Update();

	// --描画処理-- //
	void Draw();

private:
	// --コンストラクタ-- //
	GameScene();

/// --メンバ関数END-- ///
};
