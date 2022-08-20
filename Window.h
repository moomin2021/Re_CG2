#pragma once
// --Windows API-- //
#include <Windows.h>

class Window {
/// --�����o�ϐ�-- ///
public:
	// --�E�B���h�E�N���X-- //
	WNDCLASSEX w;

	// --�E�B���h�E�T�C�Y-- //
	RECT wrc;

	// --�E�B���h�E�I�u�W�F�N�g-- //
	HWND hwnd;

	// --���b�Z�[�W-- //
	MSG msg;

private:
	// --�C���X�^���X-- //
	static Window * myInstance;

	// --�E�B���h�E�T�C�Y�c��-- //
	int windowWidth;
	int windowHeight;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�ǂݍ���-- //
	static Window * GetInstance();

	// --�C���X�^���X���-- //
	void Relese();

	// --�E�B���h�E������-- //
	void Initialize();

	// --�E�B���h�E�̏c�������Q��-- //
	int GetWidth();
	int GetHeight();

	// --�E�B���h�E�N���X�Q��-- //
	WNDCLASSEX GetWNDCLASSEX();

	// --�E�B���h�E�I�u�W�F�N�g�Q��-- //
	HWND GetHWND();

private:
	// --�R���X�g���N�^-- //
	Window();

/// --�����o�֐�END-- ///

};