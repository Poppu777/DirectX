#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>

class DirectX
{
public:
	// コンストラクタ
	DirectX();

	// デストラクタ
	~DirectX();

	// DirectXの初期化
	bool Initialize(HWND hwnd);

private:
	// デバッグレイヤー
	void EnableDebugLayer();

	// DXGIファクトリーの作成
	bool CreateFactory();

	// アダプターの選択
	bool GetHardwareAdapter();

	// DirectX12デバイスの作成
	bool CreateD3D12Device();

	// コマンドキューの作成
	bool CreateCommandQueue();

	// スワップチェーンの作成
	bool CreateSwapChain(HWND hwnd);


private:
	// DirectXで使用するオブジェクト
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;
	ID3D12Device* device;
	ID3D12CommandQueue* commandQueue;
	IDXGISwapChain3* swapChain;
};