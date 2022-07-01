#include "Window.h"

// �E�B���h�E�v���V�[�W���i�V�X�e�����b�Z�[�W���������邽�߂̊֐��j
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		// �E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// --�R���X�g���N�^-- //
Window::Window() : windowWidth(1280), windowHeight(720), w{}, wrc{}, hwnd{}, msg{} {}

// --�f�X�g���N�^-- //
Window::~Window() {

	// --�E�B���h�E�N���X��o�^����-- //
	UnregisterClass(w.lpszClassName, w.hInstance);
}

// --�E�B���h�E������-- //
void Window::WindowInitialize() {

	// --�E�B���h�E�N���X�̐ݒ�-- //
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	// --�E�B���h�E�N���X��OS�ɓo�^����-- //
	RegisterClassEx(&w);

	// --�E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }-- //
	wrc = { 0, 0, windowWidth, windowHeight };

	// --�E�B���h�E�I�u�W�F�N�g�̐���-- //
	hwnd = CreateWindow(
		w.lpszClassName,// ------> �N���X��
		L"DirectXGame",// -------> �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,// --> �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,// --------> �\��X���W(OS�ɔC����)
		CW_USEDEFAULT,// --------> �\��Y���W(OS�ɔC����)
		wrc.right - wrc.left,// -> �E�B���h�E����
		wrc.bottom - wrc.top,// -> �E�B���h�E�c��
		nullptr,// --------------> �e�E�B���h�E�n���h��
		nullptr,// --------------> ���j���[�n���h��
		w.hInstance,// ----------> �Ăяo���A�v���P�[�V�����n���h��
		nullptr);// -------------> �I�v�V����

	// --�E�B���h�E��\����Ԃɂ���-- //
	ShowWindow(hwnd, SW_SHOW);

	// --�����ŃT�C�Y��␳����-- //
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
}

// --�E�B���h�E�̏c�������Q��-- //

// --����
int Window::GetWidth() { return windowWidth; }

// --�c��
int Window::GetHeight() { return windowHeight; }