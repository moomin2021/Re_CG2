#pragma once

// --入力クラス-- //
#include "Input.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --オブジェクトクラス-- //
#include "Object3D.h"

// --カメラクラス-- //
#include "Camera.h"

// --平行光源クラス-- //
#include "DirectionalLight.h"

// --点光源クラス-- //
#include "PointLight.h"

// --ベクター-- //
#include <vector>

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

	// --平行光源クラス-- //
	DirectionalLight* dirLight;

	// --点光源クラス-- //
	PointLight* pointLight;

	// --テクスチャハンドル保存用変数-- //
	int marioGraph;
	int reimuGraph;

	// --オブジェクトクラス-- //
	Object* floor;
	Object* object[2];

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
