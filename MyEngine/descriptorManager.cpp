#include "descriptorManager.h"

DescriptorManager::DescriptorManager()
{
}

DescriptorManager::~DescriptorManager()
{
}

bool DescriptorManager::CreateRTVDescriptorHeap(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name)
{
	bool created = false;

	Descriptor desc(DESCRIPTOR_TYPE::RTV);
	created = desc.Create(device, swapChain, name);

	mRTVDescriptorHeaps.emplace(name, desc);

	return created;
}

Descriptor DescriptorManager::GetRTVDescriptor(std::wstring name) const
{
	return mRTVDescriptorHeaps.at(name);
};
