#include "descriptor_heap.h"

DescriptorHeap::DescriptorHeap()
{
	descriptorHeap = nullptr;
	descriptorSize = 0;
}


DescriptorHeap::~DescriptorHeap()
{
	if (descriptorHeap != nullptr)
	{
		descriptorHeap->Release();
		descriptorHeap = nullptr;
	}
}


// RTV用ディスクリプタヒープを作成
bool DescriptorHeap::Initialize(
	ID3D12Device* device,
	UINT numDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	heapDesc.NumDescriptors = numDescriptors;

	heapDesc.Type =
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	heapDesc.Flags =
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	heapDesc.NodeMask = 0;


	HRESULT hr = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(&descriptorHeap));

	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to create Descriptor Heap\n");

		return false;
	}


	descriptorSize =
		device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


	descriptorHeap->SetName(
		L"Main RTV Descriptor Heap");

	return true;
}


// ディスクリプタヒープを取得
ID3D12DescriptorHeap* DescriptorHeap::Get()
{
	return descriptorHeap;
}


// ディスクリプタ1個分のサイズを取得
UINT DescriptorHeap::GetDescriptorSize()
{
	return descriptorSize;
}