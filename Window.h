#pragma once
// --Windows API-- //
#include <Windows.h>

class Window {
/// --メンバ変数-- ///
public:
	// --ウィンドウクラス-- //
	WNDCLASSEX w;

	// --ウィンドウサイズ-- //
	RECT wrc;

	// --ウィンドウオブジェクト-- //
	HWND hwnd;

	// --メッセージ-- //
	MSG msg;

private:
	// --インスタンス-- //
	static Window * myInstance;

	// --ウィンドウサイズ縦横-- //
	int windowWidth;
	int windowHeight;

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --インスタンス読み込み-- //
	static Window * GetInstance();

	// --インスタンス解放-- //
	void Relese();

	// --ウィンドウ初期化-- //
	void Initialize();

	// --ウィンドウの縦横幅を参照-- //
	int GetWidth();
	int GetHeight();

	// --ウィンドウクラス参照-- //
	WNDCLASSEX GetWNDCLASSEX();

	// --ウィンドウオブジェクト参照-- //
	HWND GetHWND();

private:
	// --コンストラクタ-- //
	Window();

/// --メンバ関数END-- ///

};