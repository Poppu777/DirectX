#include "fence.h"

Fence::Fence()
{
	fence = nullptr;
	fenceValue = 0;
	fenceEvent = nullptr;
}


Fence::~Fence()
{
	if (fenceEvent != nullptr)
	{
		CloseHandle(fenceEvent);
		fenceEvent = nullptr;
	}

	if (fence != nullptr)
	{
		fence->Release();
		fence = nullptr;
	}
}


// フェンスを作成
bool Fence::Initialize(ID3D12Device* device)
{
	HRESULT hr = device->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&fence));

	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to create Fence\n");

		return false;
	}


	fenceValue = 1;


	fenceEvent = CreateEvent(
		nullptr,
		FALSE,
		FALSE,
		nullptr);

	if (fenceEvent == nullptr)
	{
		OutputDebugStringA(
			"Failed to create Fence Event\n");

		return false;
	}


	fence->SetName(
		L"Main Fence");

	return true;
}


// GPUの処理完了を待つ
void Fence::Wait(
	ID3D12CommandQueue* commandQueue)
{
	const UINT64 currentFenceValue =
		fenceValue;


	// GPUにフェンス値を設定
	commandQueue->Signal(
		fence,
		currentFenceValue);


	// GPUがまだ処理中の場合
	if (fence->GetCompletedValue() <
		currentFenceValue)
	{
		fence->SetEventOnCompletion(
			currentFenceValue,
			fenceEvent);

		WaitForSingleObject(
			fenceEvent,
			INFINITE);
	}


	fenceValue++;
}