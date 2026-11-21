#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>

#include "command_allocator.h"
#include "command_list.h"
#include "descriptor_heap.h"
#include "fence.h"
#include "render_target.h"


class DirectX
{
public:
	DirectX() = default;
	~DirectX();

	// DirectXの初期化
	bool Initialize(HWND hwnd);

private:
	void EnableDebugLayer();

	bool CreateFactory();

	bool GetHardwareAdapter();

	bool CreateD3D12Device();

	bool CreateCommandQueue();

	bool CreateSwapChain(HWND hwnd);


private:
	// DirectXの基本オブジェクト
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;
	ID3D12Device* device;
	ID3D12CommandQueue* commandQueue;
	IDXGISwapChain3* swapChain;


	// DirectXの描画関連クラス
	CommandAllocator commandAllocator;
	CommandList commandList;
	DescriptorHeap descriptorHeap;
	Fence fence;
	RenderTarget renderTarget;
};