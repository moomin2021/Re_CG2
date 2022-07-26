#pragma once
#include <dinput.h>
#include "Vector2.h"

enum MouseButtonName {
	M_LEFT,
	M_RIGHT,
	M_CENTER
};

class Input
{
private:
	
	// --ウィンドウクラスの名前-- //
	LPCWSTR windowClassName;

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
	IDirectInputDevice8 * keyBoardDev;
	IDirectInputDevice8* mouseDev;

public:
	// --コンストラクタ-- //
	Input();

	// --デストラクタ-- //
	~Input();

	// キーボード入力の初期化
	void InitializeInput(WNDCLASSEX w, HWND hwnd);

	// キーボード入力の更新処理
	void UpdateInput();

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
};