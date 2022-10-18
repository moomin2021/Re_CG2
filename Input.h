#pragma once
// --DirectXInput-- //
#define DIREXTINPUT_VERSION 0x0800// DirectInputのバージョン指定
#include <dinput.h>

// --Vector2クラス-- //
#include "Vector2.h"

// --ComPtr用-- //
#include <wrl.h>

// --WindowsAPIクラス-- //
#include "Window.h"

enum MouseButtonName {
	M_LEFT,
	M_RIGHT,
	M_CENTER
};

class Input
{
/// --メンバ変数-- ///
public:
	// --namespaceの省略-- //
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	// --インスタンス-- //
	static Input* myInstance;

	// --WindowsAPIクラス-- //
	Window* win;

	// --現在のキーボードの情報-- //
	UINT8 keys[256];

	// --前フレームのキーボードの情報-- //
	UINT8 oldKeys[256];

	// --現在マウスの入力状態-- //
	DIMOUSESTATE mouse;

	// --前フレームのマウスの入力状態-- //
	DIMOUSESTATE oldMouse;

	POINT p;

	// --各デバイス-- //
	ComPtr<IDirectInputDevice8> keyBoardDev;
	ComPtr<IDirectInputDevice8> mouseDev;

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:

	// --インスタンス読み込み-- //
	static Input* GetInstance();

	// --インスタンス解放-- //
	void Relese();

	// --デストラクタ-- //
	~Input();

	// キーボード入力の初期化
	void Initialize();

	// キーボード入力の更新処理
	void Update();

	/// <summary>
	/// 指定されたキーが押されていたら、1を返し、そうでなかったら0を返す
	/// </summary>
	/// <param name="key"> 判定したいキー </param>
	/// <returns></returns>
	bool PushKey(UINT8 key);

	/// <summary>
	/// 指定されたキーが押された瞬間だったら、1を返し、そうでなかったら0を返す
	/// </summary>
	/// <param name="key"> 判定したいキー </param>
	/// <returns></returns>
	bool TriggerKey(UINT8 key);

	/// <summary>
	/// 指定されたマウスのボタンが押されていたら、1を返し、そうでなかった場合0を返す
	/// </summary>
	/// <param name="button"> 判定したいマウスボタン </param>
	/// <returns></returns>
	bool PushMouseButton(MouseButtonName button);

	/// <summary>
	/// 指定されたマウスのボタンが押された瞬間だったら、1を返し、そうでなかった場合0を返す
	/// </summary>
	/// <param name="button"> 判定したいマウスボタン </param>
	/// <returns></returns>
	bool TriggerMouseButton(MouseButtonName button);

	// --マウスの位置-- //
	Vector2 MousePos();

	// --マウスの移動量-- //
	Vector2 GetMouseVelosity();

private:
	// --コンストラクタ-- //
	Input();

	/// --メンバ関数END-- ///
};