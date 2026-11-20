#pragma once

#include <d3d12.h>

class CommandList
{
public:
	CommandList();
	~CommandList();

	// コマンドリストを作成
	bool Initialize(
		ID3D12Device* device,
		ID3D12CommandAllocator* commandAllocator);

	// コマンドリストを閉じる
	void Close();

	// コマンドリストをリセット
	void Reset(ID3D12CommandAllocator* commandAllocator);

	// コマンドリストを取得
	ID3D12GraphicsCommandList* Get();

private:
	ID3D12GraphicsCommandList* commandList;
};