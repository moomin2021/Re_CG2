#include "Input.h"
#include <cassert>
#define DIREXTINPUT_VERSION 0x0800// DirectInput�̃o�[�W�����w��
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input::Input() : keys{}, oldKeys{}, keyBoard(nullptr) {}

void Input::InitializeInput(WNDCLASSEX w, HWND hwnd)
{
	HRESULT result;

	// --DirectInput�̏�����-- //
	IDirectInput8 * directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void **)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

	// --�L�[�{�[�h�f�o�C�X�̐���-- //
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyBoard, NULL);
	assert(SUCCEEDED(result));

	// --���̓f�[�^�̌`���̃Z�b�g-- //
	result = keyBoard->SetDataFormat(&c_dfDIKeyboard);// �W���`��
	assert(SUCCEEDED(result));

	// --�r�����䃌�x���̃Z�b�g-- //
	result = keyBoard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));
}

void Input::UpdateInput()
{
	// --�L�[�{�[�h���̎擾�J�n-- //
	keyBoard->Acquire();

	// --�O�t���[���̃L�[�̏�Ԃ�ۑ�-- //
	for (size_t i = 0; i < 256; i++) oldKeys[i] = keys[i];

	// --�S�L�[�̓��͏�Ԃ��擾����-- //
	keyBoard->GetDeviceState(sizeof(keys), keys);
}

bool Input::PushKey(UINT8 key) { return keys[key]; }

bool Input::TriggerKey(UINT8 key) { return keys[key] && !oldKeys[key]; }