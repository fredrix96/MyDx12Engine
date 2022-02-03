#include "commandManager.h"

CommandManager::CommandManager() :
	mCommandAllocator{NULL}, mCommandList(NULL), mHr(NULL)
{
}

CommandManager::~CommandManager()
{
}

bool CommandManager::CreateCommandAllocators(ID3D12Device* device)
{
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; i++)
	{
		if (mCommandAllocator[i] != NULL)
		{
			ASSERT(false);
			return false;
		}

		mHr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator[i]));
		if (FAILED(mHr))
		{
			return false;
		}
	}

	return true;
}

bool CommandManager::CreateCommandList(ID3D12Device* device, int frameIndex)
{
	// create the command list with the first allocator
	mHr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator[frameIndex].Get(), NULL, IID_PPV_ARGS(mCommandList.GetAddressOf()));
	if (FAILED(mHr))
	{
		return false;
	}
	return true;
}

ComPtr<ID3D12CommandAllocator> CommandManager::GetCommandAllocator(int pos) const
{
	return mCommandAllocator[pos];
}

ComPtr<ID3D12GraphicsCommandList> CommandManager::GetCommandList() const
{
	return mCommandList;
}
