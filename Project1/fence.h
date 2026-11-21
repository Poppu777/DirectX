#pragma once

#include <d3d12.h>

class Fence
{
public:
	Fence() = default;
	~Fence();

	// ƒtƒFƒ“ƒX‚ğì¬
	bool Initialize(ID3D12Device* device);

	// GPU‚Ìˆ—Š®—¹‚ğ‘Ò‚Â
	void Wait(
		ID3D12CommandQueue* commandQueue);

private:
	ID3D12Fence* fence;

	UINT64 fenceValue;

	HANDLE fenceEvent;
};