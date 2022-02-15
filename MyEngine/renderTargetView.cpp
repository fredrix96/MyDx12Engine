#include "renderTargetView.h"

RenderTargetView::RenderTargetView(ID3D12DescriptorHeap* descriptorHeap, ID3D12Resource* resource[NUM_OF_FRAME_BUFFERS], D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc,
	int descriptorSize, std::wstring name) : DescriptorHeap(descriptorHeap, resource, descriptorHeapDesc, descriptorSize, name)
{
}

RenderTargetView::~RenderTargetView()
{
}

CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTargetView::GetHandle(int frameIndex) const
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, mDescriptorSize);
	return handle;
}

ComPtr<ID3D12Resource> RenderTargetView::GetRenderTarget(int pos) const
{
	return mResources[pos];
}
