#include "command_list.h"

CommandList::CommandList()
{
	commandList = nullptr;
}


CommandList::~CommandList()
{
	if (commandList != nullptr)
	{
		commandList->Release();
		commandList = nullptr;
	}
}


// コマンドリストを作成
bool CommandList::Initialize(
	ID3D12Device* device,
	ID3D12CommandAllocator* commandAllocator)
{
	HRESULT hr = device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator,
		nullptr,
		IID_PPV_ARGS(&commandList));

	if (FAILED(hr))
	{
		OutputDebugStringA(
			"Failed to create Command List\n");

		return false;
	}

	commandList->SetName(
		L"Main Command List");

	// 作成直後は開いているので閉じておく
	commandList->Close();

	return true;
}


// コマンドリストを閉じる
void CommandList::Close()
{
	commandList->Close();
}


// コマンドリストをリセット
void CommandList::Reset(
	ID3D12CommandAllocator* commandAllocator)
{
	commandList->Reset(
		commandAllocator,
		nullptr);
}


// コマンドリストを取得
ID3D12GraphicsCommandList* CommandList::Get()
{
	return commandList;
}