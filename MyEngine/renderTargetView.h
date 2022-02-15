#pragma once

#include "descriptorHeap.h"

class RenderTargetView : public DescriptorHeap
{
private:
public:

	RenderTargetView(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource[NUM_OF_FRAME_BUFFERS], D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc, 
		int descriptorSize, std::wstring name);
	~RenderTargetView();

	bool Create(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name);

	ComPtr<ID3D12Resource> GetRenderTarget(int pos) const;
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetHandle(int frameIndex) const;
};