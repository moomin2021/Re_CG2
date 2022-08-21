#pragma once

// --入力クラス-- //
#include "Input.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --オブジェクトクラス-- //
#include "Object3D.h"

// --カメラクラス-- //
#include "Camera.h"

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

	// --テクスチャハンドル保存用変数-- //
	int marioGraph;
	int reimuGraph;

	// --オブジェクトクラス-- //
	Object* object;

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
