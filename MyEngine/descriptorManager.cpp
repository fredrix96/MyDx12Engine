#include "descriptorManager.h"

DescriptorManager::DescriptorManager()
{
}

DescriptorManager::~DescriptorManager()
{
}

bool DescriptorManager::CreateRTVDescriptorHeap(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name)
{
	Descriptor desc(DESCRIPTOR_TYPE::RTV);
	bool isCreated = desc.Create(device, swapChain, name);

	if (isCreated)
	{
		mRTVDescriptorHeaps.emplace(name, desc);
	}

	return isCreated;
}

Descriptor DescriptorManager::GetRTVDescriptor(std::wstring name) const
{
	return mRTVDescriptorHeaps.at(name);
};
