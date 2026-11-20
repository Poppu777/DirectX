#pragma once

#include <d3d12.h>

class DescriptorHeap
{
public:
	DescriptorHeap();
	~DescriptorHeap();

	// RTV用ディスクリプタヒープを作成
	bool Initialize(
		ID3D12Device* device,
		UINT numDescriptors);

	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* Get();

	// ディスクリプタ1個分のサイズを取得
	UINT GetDescriptorSize();

private:
	ID3D12DescriptorHeap* descriptorHeap;

	UINT descriptorSize;
};