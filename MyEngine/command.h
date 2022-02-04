#pragma once

#include "stdafx.h"

class Command
{
private:
	ComPtr<ID3D12CommandAllocator> mCommandAllocator[NUM_OF_FRAME_BUFFERS]; // we want enough allocators for each buffer * number of threads (we only have one thread)
	ComPtr<ID3D12GraphicsCommandList> mCommandList; // a command list we can record commands into, then execute them to render the frame

	std::wstring mNameAlloc, mNameList;

	HRESULT mHr;
public:
	Command();
	~Command();

	bool CreateCommandAllocators(ID3D12Device* device, std::wstring name);
	bool CreateCommandList(ID3D12Device* device, int frameIndex, std::wstring name);

	ComPtr<ID3D12CommandAllocator> GetCommandAllocator(int pos) const;
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() const;
};