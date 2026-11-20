#include "command_allocator.h"

CommandAllocator::CommandAllocator()
{
	commandAllocator = nullptr;
}


CommandAllocator::~CommandAllocator()
{
	if (commandAllocator != nullptr)
	{
		commandAllocator->Release();
		commandAllocator = nullptr;
	}
}


// コマンドアロケータを作成
bool CommandAllocator::Initialize(ID3D12Device* device)
{
	HRESULT hr = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));

	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to create Command Allocator\n");

		return false;
	}

	commandAllocator->SetName(
		L"Main Command Allocator");

	return true;
}


// コマンドアロケータをリセット
void CommandAllocator::Reset()
{
	commandAllocator->Reset();
}


// コマンドアロケータを取得
ID3D12CommandAllocator* CommandAllocator::Get()
{
	return commandAllocator;
}