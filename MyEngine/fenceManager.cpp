#include "fenceManager.h"

FenceManager::FenceManager() :
	mFence{ NULL }, mFenceEvent(NULL), mFenceValue{ (UINT64)DEFAULT_VALUE }, mHr(NULL)
{
}

FenceManager::~FenceManager()
{
}

bool FenceManager::CreateFences(ID3D12Device* device)
{
	// create the fences
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; i++)
	{
		if (mFence[i] != NULL)
		{
			ASSERT(false);
			return false;
		}

		mHr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence[i]));
		if (FAILED(mHr))
		{
			return false;
		}
		mFenceValue[i] = 0; // set the initial fence value to 0
	}

	// create a handle to a fence event
	mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (mFenceEvent == nullptr)
	{
		return false;
	}

	return true;
}

void FenceManager::IncrementFenceValue(int frameIndex)
{
	mFenceValue[frameIndex]++;
}

ComPtr<ID3D12Fence> FenceManager::GetFence(int pos) const
{
	return mFence[pos];
}

HANDLE & FenceManager::GetFenceEvent()
{
	return mFenceEvent;
}

UINT64 & FenceManager::GetFenceValue(int pos)
{
	return mFenceValue[pos];
}
