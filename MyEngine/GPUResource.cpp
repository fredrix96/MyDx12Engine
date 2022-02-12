#include "GPUResource.h"

GPUResource::GPUResource(ID3D12Resource* bufferResource, ID3D12Resource* bufferUploadHeap, D3D12_SUBRESOURCE_DATA resourceData, int bufferSize) :
	mBufferResource(bufferResource), mBufferUploadHeap(bufferUploadHeap), mResourceData(resourceData), mBufferSize(bufferSize)
{
}

GPUResource::~GPUResource()
{
}

void GPUResource::CopyDataFromUploadHeapToDefaultHeap(ID3D12GraphicsCommandList* cmdList)
{
	// we are now creating a command with the command list to copy the data from
	// the upload heap to the default heap
	UpdateSubresources(cmdList, mBufferResource.Get(), mBufferUploadHeap.Get(), 0, 0, 1, &mResourceData);
}

ComPtr<ID3D12Resource> GPUResource::GetBufferResource() const
{
	return mBufferResource;
}
