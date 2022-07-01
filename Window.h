#pragma once
#include <Windows.h>

class Window {

private:

	// --ウィンドウサイズ縦横-- //
	int windowWidth;
	int windowHeight;

public:

	// --ウィンドウクラス-- //
	WNDCLASSEX w;

	// --ウィンドウサイズ-- //
	RECT wrc;

	// --ウィンドウオブジェクト-- //
	HWND hwnd;

	// --メッセージ-- //
	MSG msg;

	// --コンストラクタ-- //
	Window();

	// --デストラクタ-- //
	~Window();

	// --ウィンドウ初期化-- //
	void WindowInitialize();

	// --ウィンドウの縦横幅を参照-- //
	int GetWidth();
	int GetHeight();

};