#include "render_target.h"

RenderTarget::RenderTarget()
{
	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		renderTargets[i] = nullptr;
	}
}


RenderTarget::~RenderTarget()
{
	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		if (renderTargets[i] != nullptr)
		{
			renderTargets[i]->Release();
			renderTargets[i] = nullptr;
		}
	}
}


// レンダーターゲットを作成
bool RenderTarget::Initialize(
	ID3D12Device* device,
	IDXGISwapChain3* swapChain,
	ID3D12DescriptorHeap* descriptorHeap,
	UINT descriptorSize)
{
	// RTVヒープの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		descriptorHeap->GetCPUDescriptorHandleForHeapStart();


	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		// スワップチェーンからバックバッファを取得
		HRESULT hr = swapChain->GetBuffer(
			i,
			IID_PPV_ARGS(&renderTargets[i]));

		if (FAILED(hr))
		{
			OutputDebugStringA(
				"Failed to get SwapChain Buffer\n");

			return false;
		}


		// RTVを作成
		device->CreateRenderTargetView(
			renderTargets[i],
			nullptr,
			handle);


		// 次のディスクリプタへ移動
		handle.ptr += descriptorSize;
	}

	return true;
}


// 現在のバックバッファを取得
ID3D12Resource* RenderTarget::GetCurrentRenderTarget(
	UINT index)
{
	if (index >= BUFFER_COUNT)
	{
		return nullptr;
	}

	return renderTargets[index];
}