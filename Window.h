#pragma once
#include <Windows.h>

class Window {

private:

	// --�E�B���h�E�T�C�Y�c��-- //
	int windowWidth;
	int windowHeight;

public:

	// --�E�B���h�E�N���X-- //
	WNDCLASSEX w;

	// --�E�B���h�E�T�C�Y-- //
	RECT wrc;

	// --�E�B���h�E�I�u�W�F�N�g-- //
	HWND hwnd;

	// --���b�Z�[�W-- //
	MSG msg;

	// --�R���X�g���N�^-- //
	Window();

	// --�f�X�g���N�^-- //
	~Window();

	// --�E�B���h�E������-- //
	void WindowInitialize();

	// --�E�B���h�E�̏c�������Q��-- //
	int GetWidth();
	int GetHeight();

};