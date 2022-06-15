#pragma once
#include <dinput.h>

class Input
{
private:
	UINT8 keys[256];
	UINT8 oldKeys[256];

	IDirectInputDevice8 * keyBoard;

public:
	// �R���X�g���N�^
	Input();

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
};