#pragma once

#include "descriptorHeap.h"

class DepthBuffer : DescriptorHeap
{
private:
public:

	DepthBuffer(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc, int descriptorSize, std::wstring name);
	~DepthBuffer();

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetHandle() const;
};
