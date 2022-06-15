#pragma once
#include <dinput.h>

class Input
{
private:
	UINT8 keys[256];
	UINT8 oldKeys[256];

	IDirectInputDevice8 * keyBoard;

public:
	// コンストラクタ
	Input();

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
};