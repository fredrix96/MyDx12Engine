#pragma once

#include "stdafx.h"

class FenceManager
{
private:
	ComPtr<ID3D12Fence> mFence[NUM_OF_FRAME_BUFFERS];  // an object that is locked while our command list is being executed by the gpu. We need as many 
													  // as we have allocators (more if we want to know when the gpu is finished with an asset)
	HANDLE mFenceEvent; // a handle to an event when our fence is unlocked by the gpu
	UINT64 mFenceValue[NUM_OF_FRAME_BUFFERS]; // this value is incremented each frame. each fence will have its own value

	HRESULT mHr;
public:

	FenceManager();
	~FenceManager();

	bool CreateFences(ID3D12Device* device);
	void IncrementFenceValue(int frameIndex);

	ComPtr<ID3D12Fence> GetFence(int pos) const;
	HANDLE GetFenceEvent() const;
	UINT64 GetFenceValue(int pos) const;
};