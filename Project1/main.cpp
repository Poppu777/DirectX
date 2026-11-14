#include <Windows.h>
#include "DirectX.h"
#include "descriptor_heap.h"
#include "render_target.h"
#include "command_list.h"
#include "command_allocator.h"


// ４．メッセージ受信
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		// 画面を更新する処理
		return 0;

	case WM_KEYDOWN:
		// キー入力の処理
		return 0;
	}

	// 自分で処理しないメッセージはWindowsに任せる
	return DefWindowProc(
		hwnd,
		uMsg,
		wParam,
		lParam);
}


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// ========================================
	// 1. ウィンドウクラス登録
	// ========================================

	WNDCLASS wc{};

	wc.lpfnWndProc = WindowProc;

	wc.hInstance = hInstance;

	wc.lpszClassName = "GameWindow";

	wc.hCursor =
		LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground =
		(HBRUSH)(COLOR_WINDOW + 1);


	RegisterClass(&wc);


	// ========================================
	// 2. ウィンドウ作成
	// ========================================

	HWND hwnd = CreateWindow(
		"GameWindow",
		"SHADOW",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000,
		800,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	ShowWindow(hwnd, nCmdShow);


	// ========================================
	// 3. DirectX初期化
	// ========================================

	DirectX directX;

	if (!directX.Initialize(hwnd))
	{
		MessageBox(
			hwnd,
			"DirectXの初期化に失敗しました。",
			"Error",
			MB_OK);

		return -1;
	}


	// ========================================
	// 4. メッセージループ
	// ========================================

	MSG msg{};

	while (GetMessage(
		&msg,
		NULL,
		0,
		0))
	{
		TranslateMessage(&msg);

		DispatchMessage(&msg);
	}


	return 0;
}