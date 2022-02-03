#include "commandQueueManager.h"

CommandQueueManager::CommandQueueManager()
	: mCopyCommandQueue(NULL),
	mComputeCommandQueue(NULL),
	mDirectCommandQueue(NULL), mHr(NULL)
{
}

CommandQueueManager::~CommandQueueManager()
{
}

bool CommandQueueManager::CreateDirectCommandQueue(ID3D12Device* device)
{
	if (mDirectCommandQueue != NULL)
	{
		ASSERT(false);
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC cqDesc = {};
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // direct means the gpu can directly execute this command queue

	mHr = device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&mDirectCommandQueue)); // create the command queue
	if (FAILED(mHr))
	{
		ASSERT(false);
		return false;
	}

	return true;
}

ComPtr<ID3D12CommandQueue> CommandQueueManager::GetDirectCommandQueue() const
{
	return mDirectCommandQueue;
}
