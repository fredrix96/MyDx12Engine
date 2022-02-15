#include "descriptorHeap.h"

DescriptorHeap::DescriptorHeap(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc,
	int descriptorSize, std::wstring name) : mDescriptorHeap(descriptorHeap), mResource(resource), mDescriptorHeapDesc(descriptorHeapDesc),
	mDescriptorSize(descriptorSize), mName(name), mResources{ NULL }
{
}

DescriptorHeap::DescriptorHeap(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource[NUM_OF_FRAME_BUFFERS], D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc,
	int descriptorSize, std::wstring name) : mDescriptorHeap(descriptorHeap), mDescriptorHeapDesc(descriptorHeapDesc),
	mDescriptorSize(descriptorSize), mName(name), mResource(NULL)
{
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; i++)
	{
		mResources[i] = resource[i];
	}
}

DescriptorHeap::~DescriptorHeap()
{
}

ComPtr<ID3D12DescriptorHeap> DescriptorHeap::GetDescriptorHeap() const
{
	return mDescriptorHeap;
}

std::wstring DescriptorHeap::GetName() const
{
	return mName;
}
