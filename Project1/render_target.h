#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	// レンダーターゲットを作成
	bool Initialize(
		ID3D12Device* device,
		IDXGISwapChain3* swapChain,
		ID3D12DescriptorHeap* descriptorHeap,
		UINT descriptorSize);

	// 現在のバックバッファを取得
	ID3D12Resource* GetCurrentRenderTarget(
		UINT index);

private:
	static const UINT BUFFER_COUNT = 2;

	ID3D12Resource* renderTargets[BUFFER_COUNT];
};