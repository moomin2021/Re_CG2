#include "Window.h"
#include "DXManager.h"
#include "Vertex.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;

// --シェーダの読み込みとコンパイル用-- //
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXteX.h>

#include "Util.h"

// --ゲームシーンクラス-- //
#include "GameScene.h"

// --テクスチャクラス-- //
#include "Texture.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// --ウィンドウ生成クラス-- //
	Window * win = Window::GetInstance();

	// --ウィンドウの初期化-- //
	win->Initialize();

	// --DirectX初期化クラス-- //
	DXManager * dxMa = DXManager::GetInstance();

	// --DirectX初期化処理-- //
	dxMa->Initialize(win->GetHWND(), win->GetWidth(), win->GetHeight());

	// --テクスチャクラスのインスタンス生成-- //
	Texture* texture = Texture::GetInstance();

	// --テクスチャクラスの初期化処理-- //
	texture->Initialize(dxMa->GetDevice());

	// --ゲームシーンクラスのインスタンス生成-- //
	GameScene* gameScene = GameScene::GetInstance();

	// --ゲームシーン初期化処理-- //
	gameScene->Initialize();

#pragma endregion

#pragma region

	// --ゲームループ-- //
	while (true)
	{

		// --メッセージがあるか-- //
		if (PeekMessage(&win->msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&win->msg); // キー入力メッセージの処理
			DispatchMessage(&win->msg); // プロシージャにメッセージを送る
		}

		// --✖ボタンで終了メッセージが来たらゲームループを抜ける-- //
		if (win->msg.message == WM_QUIT)
		{
			break;
		}

		////////////////////////////
		// --DirectX毎フレーム処理-- //
		////////////////////////////

#pragma region

		// --ゲームシーン更新処理-- //
		gameScene->Update();

		// --グラフィックスコマンドスタート-- //
		dxMa->GraphicsCommandStart(texture->srvHeap);

#pragma endregion
		/// --END-- ///

		/// --描画処理-- //
#pragma region

		// --ゲームシーン描画処理-- //
		gameScene->Draw();

#pragma endregion
		/// --END-- ///

		// --グラフィックスコマンドエンド-- //
		dxMa->GraphicsCommandEnd();

#pragma endregion
		/// --END-- ///

#pragma endregion
	}

	// --ウィンドウクラスを登録解除-- //
	UnregisterClass(win->w.lpszClassName, win->w.hInstance);

	win->Relese();
	dxMa->Relese();
	gameScene->Relese();
	texture->Relese();

	return 0;
}