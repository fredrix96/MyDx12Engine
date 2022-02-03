#pragma once

#include "stdafx.h"

// Container for command lists
class CommandQueueManager
{
private:
	// Can be used to issue commands to copy resource data (CPU -> GPU, GPU -> GPU, GPU -> CPU)
	ComPtr<ID3D12CommandQueue> mCopyCommandQueue;

	// Can do everything a Copy queue can do and issue compute (dispatch) commands.
	ComPtr<ID3D12CommandQueue> mComputeCommandQueue;

	// Can do everything a Copy and a Compute queue can do and issue draw commands.
	ComPtr<ID3D12CommandQueue> mDirectCommandQueue;

	HRESULT mHr;
public:

	CommandQueueManager();
	~CommandQueueManager();

	bool CreateDirectCommandQueue(ID3D12Device* device);

	ComPtr<ID3D12CommandQueue> GetDirectCommandQueue() const;
};
