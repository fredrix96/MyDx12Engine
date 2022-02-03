#pragma once

#include "stdafx.h"

// RTV descriptor heaps are called "rtvDescriptor_" + "name"

class Descriptor
{
private:
	ComPtr<ID3D12DescriptorHeap> mDescriptorHeap; // descriptor heaps hold resources like the render targets
	ComPtr<ID3D12Resource> mRenderTargets[NUM_OF_FRAME_BUFFERS]; // number of render targets equal to buffer count

	D3D12_DESCRIPTOR_HEAP_DESC mDescriptorHeapDesc;

	HRESULT mHr;

	int mDescriptorSize; // size of the descriptor on the device (all front and back buffers will be the same size)
	std::wstring mName;

	DESCRIPTOR_TYPE mType;
public:

	Descriptor(DESCRIPTOR_TYPE type);
	~Descriptor();

	bool Create(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name);

	std::wstring GetName() const;
	ComPtr<ID3D12Resource> GetRenderTarget(int pos) const;
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetHandle(int frameIndex) const;
};