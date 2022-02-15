#pragma once

#include "stdafx.h"

class DescriptorHeap
{
protected:
	ComPtr<ID3D12DescriptorHeap> mDescriptorHeap; // descriptor heaps hold resources
	ComPtr<ID3D12Resource> mResource;
	ID3D12Resource* mResources[NUM_OF_FRAME_BUFFERS];

	D3D12_DESCRIPTOR_HEAP_DESC mDescriptorHeapDesc;

	int mDescriptorSize; // size of the descriptor on the device (all front and back buffers will be the same size)
	std::wstring mName;
public:

	DescriptorHeap(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc,
		int descriptorSize, std::wstring name);
	DescriptorHeap(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource[NUM_OF_FRAME_BUFFERS], D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc,
		int descriptorSize, std::wstring name);
	~DescriptorHeap();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const;
	std::wstring GetName() const;
};
