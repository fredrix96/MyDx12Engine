#include "descriptorManager.h"

DescriptorManager::DescriptorManager() :
	mHr(NULL)
{
}

DescriptorManager::~DescriptorManager()
{
}

bool DescriptorManager::CreateRTVDescriptorHeap(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name)
{
	ID3D12DescriptorHeap* descriptorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};

	// describe an rtv descriptor heap and create
	descriptorHeapDesc.NumDescriptors = NUM_OF_FRAME_BUFFERS; // number of descriptors for this heap.
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

	// This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
	// otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	mHr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	if (FAILED(mHr))
	{
		ASSERT(false);
		return false;
	}

	std::wstring tmpName = L"rtvDescriptor_" + name;
	descriptorHeap->SetName(tmpName.c_str());

	// get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
	// descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
	// device to give us the size. we will use this size to increment a descriptor handle offset
	int descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
	// but we cannot literally use it like a c++ pointer.
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
	ID3D12Resource* renderTargets[NUM_OF_FRAME_BUFFERS];
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; i++)
	{
		// first we get the n'th buffer in the swap chain and store it in the n'th
		// position of our ID3D12Resource array
		mHr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
		if (FAILED(mHr))
		{
			ASSERT(false);
			return false;
		}

		// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
		device->CreateRenderTargetView(renderTargets[i], nullptr, handle);

		// we increment the rtv handle by the rtv descriptor size we got above
		handle.Offset(1, descriptorSize);
	}

	RenderTargetView rtv(descriptorHeap, renderTargets, descriptorHeapDesc, descriptorSize, name);

	mRTVMap.emplace(name, rtv);

	return true;
}

bool DescriptorManager::CreateDepthBuffer(ID3D12Device* device, int windowWidth, int windowHeight, std::wstring name)
{
	ID3D12Resource* depthStencilBuffer; // This is the memory for our depth buffer
	ID3D12DescriptorHeap* descriptorHeap; // This is a heap for our depth/stencil buffer descriptor

	// create a depth stencil descriptor heap so we can get a pointer to the depth stencil buffer
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	mHr = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	if (FAILED(mHr))
	{
		ASSERT(false);
		return false;
		//gWindowRunning = false;
	}

	int descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, windowWidth, windowHeight, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&depthStencilBuffer)
	);

	std::wstring tmpName = L"depthDescriptor__" + name;
	descriptorHeap->SetName(tmpName.c_str());

	device->CreateDepthStencilView(depthStencilBuffer, &depthStencilDesc, descriptorHeap->GetCPUDescriptorHandleForHeapStart());

	DepthBuffer tmpBuffer(descriptorHeap, depthStencilBuffer, dsvHeapDesc, descriptorSize, name);

	mDepthBufferMap.emplace(name, tmpBuffer);

	return true;
}

RenderTargetView & DescriptorManager::GetRTVDescriptor(std::wstring name)
{
	return mRTVMap.at(name);
}
DepthBuffer & DescriptorManager::GetDepthBuffer(std::wstring name)
{
	return mDepthBufferMap.at(name);
}
;
