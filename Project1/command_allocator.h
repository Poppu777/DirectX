#pragma once

#include <d3d12.h>

class CommandAllocator
{
public:
	CommandAllocator();
	~CommandAllocator();

	// コマンドアロケータを作成
	bool Initialize(ID3D12Device* device);

	// コマンドアロケータをリセット
	void Reset();

	// コマンドアロケータを取得
	ID3D12CommandAllocator* Get();

private:
	ID3D12CommandAllocator* commandAllocator;
};