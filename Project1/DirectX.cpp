#include "DirectX.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")


// コンストラクタ
DirectX::DirectX()
{
	factory = nullptr;
	adapter = nullptr;
	device = nullptr;
	commandQueue = nullptr;
	swapChain = nullptr;
}


// デストラクタ
DirectX::~DirectX()
{
	if (swapChain != nullptr)
	{
		swapChain->Release();
		swapChain = nullptr;
	}

	if (commandQueue != nullptr)
	{
		commandQueue->Release();
		commandQueue = nullptr;
	}

	if (device != nullptr)
	{
		device->Release();
		device = nullptr;
	}

	if (adapter != nullptr)
	{
		adapter->Release();
		adapter = nullptr;
	}

	if (factory != nullptr)
	{
		factory->Release();
		factory = nullptr;
	}
}


// DirectXの初期化
bool DirectX::Initialize(HWND hwnd)
{
	// 1. デバッグレイヤー
	EnableDebugLayer();

	// 2. DXGIファクトリー
	if (!CreateFactory())
	{
		return false;
	}

	// 3. アダプター
	if (!GetHardwareAdapter())
	{
		return false;
	}

	// 4. DirectX12デバイス
	if (!CreateD3D12Device())
	{
		return false;
	}

	// 5. コマンドキュー
	if (!CreateCommandQueue())
	{
		return false;
	}

	// 6. スワップチェーン
	if (!CreateSwapChain(hwnd))
	{
		return false;
	}

	return true;
}


// デバッグレイヤー
void DirectX::EnableDebugLayer()
{
#if defined(_DEBUG)

	// デバッグインターフェースを取得
	ID3D12Debug* debugController = nullptr;

	if (SUCCEEDED(
		D3D12GetDebugInterface(
			IID_PPV_ARGS(&debugController))))
	{
		// デバッグレイヤーを有効化
		debugController->EnableDebugLayer();

		// より詳細な検証を有効化
		ID3D12Debug1* debugController1 = nullptr;

		if (SUCCEEDED(
			debugController->QueryInterface(
				IID_PPV_ARGS(&debugController1))))
		{
			debugController1->SetEnableGPUBasedValidation(TRUE);

			debugController1->Release();
		}

		debugController->Release();
	}

#endif
}


// DXGIファクトリーの作成
bool DirectX::CreateFactory()
{
	UINT createFactoryFlags = 0;

#if defined(_DEBUG)

	// デバッグビルドではデバッグフラグを設定
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

#endif

	HRESULT hr = CreateDXGIFactory2(
		createFactoryFlags,
		IID_PPV_ARGS(&factory));

	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to create DXGI Factory\n");

		return false;
	}

	return true;
}


// アダプター選択と取得
bool DirectX::GetHardwareAdapter()
{
	// ハードウェアアダプターを順番に検索
	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		adapter = nullptr;

		if (DXGI_ERROR_NOT_FOUND ==
			factory->EnumAdapters1(
				adapterIndex,
				&adapter))
		{
			break;
		}

		// アダプター情報を取得
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		// ソフトウェアアダプターをスキップ
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			adapter->Release();
			adapter = nullptr;

			continue;
		}

		// DirectX12対応テスト
		if (SUCCEEDED(
			D3D12CreateDevice(
				adapter,
				D3D_FEATURE_LEVEL_11_0,
				__uuidof(ID3D12Device),
				nullptr)))
		{
			return true;
		}

		adapter->Release();
		adapter = nullptr;
	}

	return false;
}


// DirectX12デバイス作成
bool DirectX::CreateD3D12Device()
{
	HRESULT hr = E_FAIL;

	// ハードウェアアダプターでデバイス作成
	if (adapter != nullptr)
	{
		hr = D3D12CreateDevice(
			adapter,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&device));
	}

	if (FAILED(hr))
	{
		// フォールバック：WARP
		IDXGIAdapter* warpAdapter = nullptr;

		hr = factory->EnumWarpAdapter(
			IID_PPV_ARGS(&warpAdapter));

		if (SUCCEEDED(hr))
		{
			hr = D3D12CreateDevice(
				warpAdapter,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&device));

			warpAdapter->Release();
		}

		if (FAILED(hr))
		{
			OutputDebugStringA(
				"Failed to create D3D12 Device\n");

			return false;
		}

		OutputDebugStringA(
			"Using WARP adapter\n");
	}

	return true;
}


// コマンドキューの作成
bool DirectX::CreateCommandQueue()
{
	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};

	queueDesc.Type =
		D3D12_COMMAND_LIST_TYPE_DIRECT;

	queueDesc.Priority =
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

	queueDesc.Flags =
		D3D12_COMMAND_QUEUE_FLAG_NONE;

	queueDesc.NodeMask = 0;

	HRESULT hr =
		device->CreateCommandQueue(
			&queueDesc,
			IID_PPV_ARGS(&commandQueue));

	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to create Command Queue\n");

		return false;
	}

	commandQueue->SetName(
		L"Main Command Queue");

	return true;
}


// スワップチェーンの作成
bool DirectX::CreateSwapChain(HWND hwnd)
{
	// スワップチェーンの詳細設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.BufferCount = 2;

	swapChainDesc.Width = 1280;

	swapChainDesc.Height = 720;

	swapChainDesc.Format =
		DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.SwapEffect =
		DXGI_SWAP_EFFECT_FLIP_DISCARD;

	swapChainDesc.SampleDesc.Count = 1;


	IDXGISwapChain1* swapChain1 = nullptr;

	HRESULT hr =
		factory->CreateSwapChainForHwnd(
			commandQueue,
			hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain1);

	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to create Swap Chain\n");

		return false;
	}


	// SwapChain3を取得
	hr = swapChain1->QueryInterface(
		IID_PPV_ARGS(&swapChain));

	swapChain1->Release();


	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to cast to SwapChain3\n");

		return false;
	}

	return true;
}