#include "depthBuffer.h"

DepthBuffer::DepthBuffer(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc, 
	int descriptorSize, std::wstring name) : DescriptorHeap(descriptorHeap, resource, descriptorHeapDesc, descriptorSize, name)
{
}

DepthBuffer::~DepthBuffer()
{
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DepthBuffer::GetHandle() const
{
	// Get a handle to the depth/stencil buffer
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	return handle;
}
