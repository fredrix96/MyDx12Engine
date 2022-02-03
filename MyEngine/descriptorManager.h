#pragma once

#include "descriptor.h"

class DescriptorManager
{
private:
	std::map<std::wstring, Descriptor> mRTVDescriptorHeaps;
public:

	DescriptorManager();
	~DescriptorManager();
	
	bool CreateRTVDescriptorHeap(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name);

	Descriptor GetRTVDescriptor(std::wstring name) const;
};
