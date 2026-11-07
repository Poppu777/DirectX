#include <Windows.h> // WinMain を使うためのヘッダ
#include<d3d12.h>    // DirectX12 本体を利用する為のヘッダ
#include<dxgi1_4.h>	 // Windows グラフィックス基盤機能を利用する為のヘッダ
#include "DirectX.h"
#include <wrl/client.h>

#pragma comment(lib,"d3d12.lib")	// DirectX12 の本体処理を利用する為に必要なライブラリ
#pragma comment(lib,"dxgi.lib")		// 画面への表示や管理を行う機能を利用する為のライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectX12 オブジェクトを利用する為に必要なライブラリ

// ４．メッセージ受信
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:	// ×ボタンが押された時
		PostQuitMessage(0);	// 「アプリを終了します」とWindowsに通知
		return 0;
	case WM_PAINT:		// ウィンドウの再描画が必要な時
		// 画面を更新する処理をここに書く
		return 0;
	case WM_KEYDOWN:	// キーが押された時
		// キー入力の処理をここに書く
		return 0;

	}
	// 自分で処理しないメッセージはWindowsに任せる
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



int WINAPI WinMain(
	HINSTANCE hInstance,		// アプリケーションの識別番号
	HINSTANCE hPrevInstance,	// 基本使わなくていい
	LPSTR lpCmdLine,			// コマンドライン引数（起動時のオプション）
	int nCmdShow				// ウィンドウの表示方法（最大化、最小化など）
	
)
{
	// ここにメインの処理を書く
	// １．ウィンドウクラス登録
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;					// ウィンドウプロシージャを指定
	wc.hInstance = hInstance;						// アプリケーションインスタンス
	wc.lpszClassName = "GameWindow";				// ウィンドウクラス名
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		// マウスカーソル
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// 背景色

	RegisterClass(&wc);  // Windowsに登録

	// ２．ウィンドウ作成
	HWND hwnd = CreateWindow(
		"GameWindow",					// ウィンドウクラス名
		"SHADOW",						// ウィンドウタイトル
		WS_OVERLAPPEDWINDOW,			// ウィンドウスタイル
		CW_USEDEFAULT, CW_USEDEFAULT,	// 位置（自動）
		1000, 800,						// サイズ（幅、高さ）
		NULL, NULL,						// 親ウィンドウ、メニュー
		hInstance,						// インスタンス
		NULL							// 追加データ
	);

	ShowWindow(hwnd, nCmdShow); // ウィンドウを表示

	EnableDebugLayer();															// 1. デバッグレイヤー
	IDXGIFactory4* factory = CreateFactory();									// 2. DXGI ファクトリー
	IDXGIAdapter1* adapter = GetHardwareAdapter(factory);						// 3. アダプター
	ID3D12Device* device = CreateD3D12Device(factory, adapter);					// 4. デバイス
	ID3D12CommandQueue* commandQueue = CreateCommandQueue(device);              // 5. コマンドキュー
	IDXGISwapChain3* swapChain = CreateSwapChain(factory, commandQueue, hwnd);  // 6. スワップチェーン

	// ３．メッセージループ
	MSG msg{};
	while (GetMessage(&msg, NULL, 0,0))		// メッセージが来るまで待機
	{
		TranslateMessage(&msg);		// キーボードメッセージを使いやすい形に変換
		DispatchMessage(&msg);		// 適切なウィンドウプロシージャに送信
	}

	swapChain->Release();
	commandQueue->Release();
	device->Release();
	adapter->Release();
	factory->Release();

	return 0;
}

// デバッグレイヤー
void EnableDebugLayer()
{
#if defined(_DEBUG)
	// デバッグインターフェースを取得
	ID3D12Debug* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		// デバッグレイヤーを有効化
		debugController->EnableDebugLayer();

		// より詳細な検証を有効化（任意。動作は重くなる）
		ID3D12Debug1* debugController1 = nullptr;
		if (SUCCEEDED(debugController->QueryInterface(IID_PPV_ARGS(&debugController1))))
		{
			debugController1->SetEnableGPUBasedValidation(TRUE);
			debugController1->Release();    // 設定後は不要なので解放
		}

		debugController->Release();         // 設定後は不要なので解放
	}
#endif
}

// DXGI ファクトリーの作成
IDXGIFactory4* CreateFactory()
{
	IDXGIFactory4* factory = nullptr;
	UINT createFactoryFlags = 0;


#if defined(_DEBUG)
	// デバッグビルドではデバッグフラグを設定
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif


	HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		// エラーハンドリング：ファクトリー作成失敗
		OutputDebugStringA("Failed to create DXGI Factory\n");
		return nullptr;
	}

	return factory;
}

// アダプター選択と取得
IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory4* factory)
{
	IDXGIAdapter1* adapter = nullptr;

	// ハードウェアアダプターを順番に検索
	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		adapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &adapter))
		{
			break; // アダプターが見つからない場合は終了
		}

		// アダプター情報を取得
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		// ソフトウェアアダプターをスキップ
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			adapter->Release();
			continue;
		}

		// DirectX12対応テスト（最後の引数を nullptr にすると作成せず確認のみ行う）
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
		{
			return adapter; // 適切なアダプターを発見
		}

		adapter->Release();
	}

	return nullptr;
}

// DirectX12 デバイス作成
ID3D12Device* CreateD3D12Device(IDXGIFactory4* factory, IDXGIAdapter1* adapter)
{
	ID3D12Device* device = nullptr;
	HRESULT hr = E_FAIL;

	// ハードウェアアダプターでデバイス作成を試行
	if (adapter != nullptr)
	{
		hr = D3D12CreateDevice(
			adapter,                    // 使用するアダプター
			D3D_FEATURE_LEVEL_11_0,     // 最小機能レベル
			IID_PPV_ARGS(&device)       // 作成されるデバイス
		);
	}

	if (FAILED(hr))
	{
		// フォールバック：WARP（ソフトウェアアダプター）を試行
		IDXGIAdapter* warpAdapter = nullptr;
		hr = factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
		if (SUCCEEDED(hr))
		{
			hr = D3D12CreateDevice(warpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
			warpAdapter->Release();     // デバイス作成後は不要なので解放
		}

		if (FAILED(hr))
		{
			OutputDebugStringA("Failed to create D3D12 Device\n");
			return nullptr;
		}

		OutputDebugStringA("Using WARP adapter\n");
	}

	return device;
}

// コマンドキューの作成
ID3D12CommandQueue* CreateCommandQueue(ID3D12Device* device)
{
	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;			// 直接実行型
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// 通常優先度
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;			// 特別フラグなし
	queueDesc.NodeMask = 0;										// 単一GPU使用

	ID3D12CommandQueue* commandQueue = nullptr;
	HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

	if (FAILED(hr))
	{
		OutputDebugStringA("Failed to create Command Queue\n");
		return nullptr;
	}

	// デバッグ用の名前設定（任意だが推奨）
	commandQueue->SetName(L"Main Command Queue");    // SetName は常にワイド文字列を受け取るため L を付ける
	
	return commandQueue;
}

IDXGISwapChain3* CreateSwapChain(IDXGIFactory4* factory, ID3D12CommandQueue* commandQueue, HWND hwnd)
{
	// スワップチェーンの詳細設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;                      // ダブルバッファリング
	swapChainDesc.Width = 1280;                         // 画面幅
	swapChainDesc.Height = 720;                         // 画面高
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // ピクセルフォーマット
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // レンダーターゲット用
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // 高速切替
	swapChainDesc.SampleDesc.Count = 1;                 // マルチサンプリングなし

	IDXGISwapChain1* swapChain1 = nullptr;
	HRESULT hr = factory->CreateSwapChainForHwnd(
		commandQueue,      // コマンドキュー
		hwnd,              // ターゲットウィンドウ
		&swapChainDesc,    // 設定
		nullptr,           // フルスクリーン設定
		nullptr,           // 出力制限
		&swapChain1        // 作成されるスワップチェーン
	);

	if (FAILED(hr))
	{
		OutputDebugStringA("Failed to create Swap Chain\n");
		return nullptr;
	}

	// より高機能なインターフェースを取得
	IDXGISwapChain3* swapChain = nullptr;
	hr = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
	swapChain1->Release();

	if (FAILED(hr))
	{
		OutputDebugStringA("Failed to cast to SwapChain3\n");
		return nullptr;
	}

	return swapChain;
}

