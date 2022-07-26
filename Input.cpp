#include "Input.h"
#include <cassert>
#define DIREXTINPUT_VERSION 0x0800// DirectInput�̃o�[�W�����w��
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input::Input() : keys{}, oldKeys{}, keyBoardDev(nullptr) {}

// --�f�X�g���N�^-- //
Input::~Input() {
	/// --�L�[�{�[�h-- ///
#pragma region

	// --�L�[�{�[�h�f�o�C�X�̐��䐧��-- //
	keyBoardDev->Unacquire();

	// --�L�[�{�[�h�f�o�C�X�̉��-- //
	keyBoardDev->Release();

#pragma endregion
	/// --END-- ///

	/// --�}�E�X-- ///
#pragma region

	// --�}�E�X�f�o�C�X�̐��䐧��-- //
	mouseDev->Unacquire();

	// --�}�E�X�f�o�C�X�̉��-- //
	mouseDev->Release();

#pragma endregion
	/// --END-- ///
}

void Input::InitializeInput(WNDCLASSEX w, HWND hwnd)
{
	HRESULT result;

	// --�E�B���h�E�N���X�̖��O�������o�ϐ��ɕۑ�-- //
	windowClassName = w.lpszClassName;

	/// --DirectInput�̏�����-- ///
#pragma region

	IDirectInput8 * directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void **)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --�L�[�{�[�h-- ///
#pragma region

	// --�L�[�{�[�h�f�o�C�X�̐���-- //
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyBoardDev, NULL);
	assert(SUCCEEDED(result));

	// --�L�[�{�[�h���̓f�[�^�̌`���̃Z�b�g-- //
	result = keyBoardDev->SetDataFormat(&c_dfDIKeyboard);// �W���`��
	assert(SUCCEEDED(result));

	// --�r�����䃌�x���̃Z�b�g-- //
	result = keyBoardDev->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// --�L�[�{�[�h�f�o�C�X����J�n-- //
	keyBoardDev->Acquire();

#pragma endregion
	/// --END-- ///

	/// --�}�E�X-- ///
#pragma region

	// --�}�E�X�f�o�C�X�̐���-- //
	result = directInput->CreateDevice(GUID_SysMouse, &mouseDev, NULL);
	assert(SUCCEEDED(result));

	// --�}�E�X���̓f�[�^�̌`���̃Z�b�g-- //
	result = mouseDev->SetDataFormat(&c_dfDIMouse);// �W���`��
	assert(SUCCEEDED(result));

	// --�r�����䃌�x���̃Z�b�g-- //
	result = mouseDev->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// --�}�E�X�f�o�C�X����J�n-- //
	mouseDev->Acquire();

#pragma endregion
	/// --END-- ///

	// --DirectXInput�̉��-- //
	directInput->Release();
}

void Input::UpdateInput()
{
	/// --�L�[�{�[�h-- ///
#pragma region

	// --�O�t���[���̃L�[�̏�Ԃ�ۑ�-- //
	for (size_t i = 0; i < 256; i++) oldKeys[i] = keys[i];

	// --�S�L�[�̓��͏�Ԃ��擾����-- //
	keyBoardDev->GetDeviceState(sizeof(keys), keys);

#pragma endregion
	/// --END-- ///

	/// --�}�E�X-- ///
#pragma region

	// --�O�t���[���̃}�E�X�̓��͏�Ԃ�ۑ�-- //
	oldMouse = mouse;

	// --�}�E�X�̓��͏�Ԃ��擾-- //
	mouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);

	// --�}�E�X�̍��W���擾-- //
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

