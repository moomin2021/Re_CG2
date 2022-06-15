#include "Input.h"
#include <cassert>
#define DIREXTINPUT_VERSION 0x0800// DirectInputのバージョン指定
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input::Input() : keys{}, oldKeys{}, keyBoard(nullptr) {}

void Input::InitializeInput(WNDCLASSEX w, HWND hwnd)
{
	HRESULT result;

	// --DirectInputの初期化-- //
	IDirectInput8 * directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void **)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

	// --キーボードデバイスの生成-- //
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyBoard, NULL);
	assert(SUCCEEDED(result));

	// --入力データの形式のセット-- //
	result = keyBoard->SetDataFormat(&c_dfDIKeyboard);// 標準形式
	assert(SUCCEEDED(result));

	// --排他制御レベルのセット-- //
	result = keyBoard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));
}

void Input::UpdateInput()
{
	// --キーボード情報の取得開始-- //
	keyBoard->Acquire();

	// --前フレームのキーの状態を保存-- //
	for (size_t i = 0; i < 256; i++) oldKeys[i] = keys[i];

	// --全キーの入力状態を取得する-- //
	keyBoard->GetDeviceState(sizeof(keys), keys);
}

bool Input::PushKey(UINT8 key) { return keys[key]; }

bool Input::TriggerKey(UINT8 key) { return keys[key] && !oldKeys[key]; }