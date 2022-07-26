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
	
	// --�E�B���h�E�N���X�̖��O-- //
	LPCWSTR windowClassName;

	// --���݂̃L�[�{�[�h�̏��-- //
	UINT8 keys[256];

	// --�O�t���[���̃L�[�{�[�h�̏��-- //
	UINT8 oldKeys[256];

	// --���݃}�E�X�̓��͏��-- //
	DIMOUSESTATE mouse;

	// --�O�t���[���̃}�E�X�̓��͏��-- //
	DIMOUSESTATE oldMouse;

	POINT p;

	// --�e�f�o�C�X-- //
	IDirectInputDevice8 * keyBoardDev;
	IDirectInputDevice8* mouseDev;

public:
	// --�R���X�g���N�^-- //
	Input();

	// --�f�X�g���N�^-- //
	~Input();

	// �L�[�{�[�h���͂̏�����
	void InitializeInput(WNDCLASSEX w, HWND hwnd);

	// �L�[�{�[�h���͂̍X�V����
	void UpdateInput();

	/// <summary>
	/// �w�肳�ꂽ�L�[��������Ă�����A1��Ԃ��A�����łȂ�������0��Ԃ�
	/// </summary>
	/// <param name="key"> ���肵�����L�[ </param>
	/// <returns></returns>
	bool PushKey(UINT8 key);

	/// <summary>
	/// �w�肳�ꂽ�L�[�������ꂽ�u�Ԃ�������A1��Ԃ��A�����łȂ�������0��Ԃ�
	/// </summary>
	/// <param name="key"> ���肵�����L�[ </param>
	/// <returns></returns>
	bool TriggerKey(UINT8 key);

	/// <summary>
	/// �w�肳�ꂽ�}�E�X�̃{�^����������Ă�����A1��Ԃ��A�����łȂ������ꍇ0��Ԃ�
	/// </summary>
	/// <param name="button"> ���肵�����}�E�X�{�^�� </param>
	/// <returns></returns>
	bool PushMouseButton(MouseButtonName button);

	/// <summary>
	/// �w�肳�ꂽ�}�E�X�̃{�^���������ꂽ�u�Ԃ�������A1��Ԃ��A�����łȂ������ꍇ0��Ԃ�
	/// </summary>
	/// <param name="button"> ���肵�����}�E�X�{�^�� </param>
	/// <returns></returns>
	bool TriggerMouseButton(MouseButtonName button);

	// --�}�E�X�̈ʒu-- //
	Vector2 MousePos();

	// --�}�E�X�̈ړ���-- //
	Vector2 GetMouseVelosity();
};