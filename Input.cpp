#include "Input.h"
#include <cassert>
#define DIREXTINPUT_VERSION 0x0800// DirectInputのバージョン指定
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input::Input() : keys{}, oldKeys{}, keyBoardDev(nullptr) {}

// --デストラクタ-- //
Input::~Input() {
	/// --キーボード-- ///
#pragma region

	// --キーボードデバイスの制御制御-- //
	keyBoardDev->Unacquire();

	// --キーボードデバイスの解放-- //
	keyBoardDev->Release();

#pragma endregion
	/// --END-- ///

	/// --マウス-- ///
#pragma region

	// --マウスデバイスの制御制御-- //
	mouseDev->Unacquire();

	// --マウスデバイスの解放-- //
	mouseDev->Release();

#pragma endregion
	/// --END-- ///
}

void Input::InitializeInput(WNDCLASSEX w, HWND hwnd)
{
	HRESULT result;

	// --ウィンドウクラスの名前をメンバ変数に保存-- //
	windowClassName = w.lpszClassName;

	/// --DirectInputの初期化-- ///
#pragma region

	IDirectInput8 * directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void **)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --キーボード-- ///
#pragma region

	// --キーボードデバイスの生成-- //
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyBoardDev, NULL);
	assert(SUCCEEDED(result));

	// --キーボード入力データの形式のセット-- //
	result = keyBoardDev->SetDataFormat(&c_dfDIKeyboard);// 標準形式
	assert(SUCCEEDED(result));

	// --排他制御レベルのセット-- //
	result = keyBoardDev->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// --キーボードデバイス制御開始-- //
	keyBoardDev->Acquire();

#pragma endregion
	/// --END-- ///

	/// --マウス-- ///
#pragma region

	// --マウスデバイスの生成-- //
	result = directInput->CreateDevice(GUID_SysMouse, &mouseDev, NULL);
	assert(SUCCEEDED(result));

	// --マウス入力データの形式のセット-- //
	result = mouseDev->SetDataFormat(&c_dfDIMouse);// 標準形式
	assert(SUCCEEDED(result));

	// --排他制御レベルのセット-- //
	result = mouseDev->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// --マウスデバイス制御開始-- //
	mouseDev->Acquire();

#pragma endregion
	/// --END-- ///

	// --DirectXInputの解放-- //
	directInput->Release();
}

void Input::UpdateInput()
{
	/// --キーボード-- ///
#pragma region

	// --前フレームのキーの状態を保存-- //
	for (size_t i = 0; i < 256; i++) oldKeys[i] = keys[i];

	// --全キーの入力状態を取得する-- //
	keyBoardDev->GetDeviceState(sizeof(keys), keys);

#pragma endregion
	/// --END-- ///

	/// --マウス-- ///
#pragma region

	// --前フレームのマウスの入力状態を保存-- //
	oldMouse = mouse;

	// --マウスの入力状態を取得-- //
	mouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);

	// --マウスの座標を取得-- //
	GetCursorPos(&p);
	ScreenToClient(FindWindowW(windowClassName, nullptr), &p);

#pragma endregion
	/// --END-- ///
}

bool Input::PushKey(UINT8 key) { return keys[key]; }

bool Input::TriggerKey(UINT8 key) { return keys[key] && !oldKeys[key]; }

bool Input::PushMouseButton(MouseButtonName button) { return mouse.rgbButtons[button] == (0x80); }

bool Input::TriggerMouseButton(MouseButtonName button) {
	return mouse.rgbButtons[button] == (0x80) && !(oldMouse.rgbButtons[button] == (0x80));
}

Vector2 Input::MousePos() {
	return Vector2(p.x, p.y);
}

Vector2 Input::GetMouseVelosity() {
	return Vector2((float)mouse.lX, (float)mouse.lY);
}

