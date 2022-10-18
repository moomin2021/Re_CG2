#pragma once
// --DirectXInput-- //
#define DIREXTINPUT_VERSION 0x0800// DirectInput�̃o�[�W�����w��
#include <dinput.h>

// --Vector2�N���X-- //
#include "Vector2.h"

// --ComPtr�p-- //
#include <wrl.h>

// --WindowsAPI�N���X-- //
#include "Window.h"

enum MouseButtonName {
	M_LEFT,
	M_RIGHT,
	M_CENTER
};

class Input
{
/// --�����o�ϐ�-- ///
public:
	// --namespace�̏ȗ�-- //
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	// --�C���X�^���X-- //
	static Input* myInstance;

	// --WindowsAPI�N���X-- //
	Window* win;

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
	ComPtr<IDirectInputDevice8> keyBoardDev;
	ComPtr<IDirectInputDevice8> mouseDev;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:

	// --�C���X�^���X�ǂݍ���-- //
	static Input* GetInstance();

	// --�C���X�^���X���-- //
	void Relese();

	// --�f�X�g���N�^-- //
	~Input();

	// �L�[�{�[�h���͂̏�����
	void Initialize();

	// �L�[�{�[�h���͂̍X�V����
	void Update();

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

private:
	// --�R���X�g���N�^-- //
	Input();

	/// --�����o�֐�END-- ///
};