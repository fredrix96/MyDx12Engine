#include "descriptor.h"

Descriptor::Descriptor(DESCRIPTOR_TYPE type) :
	mDescriptorHeap(NULL), mDescriptorHeapDesc({}), mDescriptorSize(DEFAULT_VALUE),
	mHr(NULL), mName(), mType(type)
{
}

Descriptor::~Descriptor()
{
}

bool Descriptor::Create(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name)
{
	// describe an rtv descriptor heap and create
	mDescriptorHeapDesc.NumDescriptors = NUM_OF_FRAME_BUFFERS; // number of descriptors for this heap.

	if (mType == DESCRIPTOR_TYPE::RTV)
	{
		mDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap
	}

	// This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
	// otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	mDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	mHr = device->CreateDescriptorHeap(&mDescriptorHeapDesc, IID_PPV_ARGS(&mDescriptorHeap));
	if (FAILED(mHr))
	{
		ASSERT(false);
		return false;
	}

	mName = L"rtvDescriptor_" + name;
	mDescriptorHeap->SetName(mName.c_str());

	if (mType == DESCRIPTOR_TYPE::RTV)
	{
		// get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
		// descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
		// device to give us the size. we will use this size to increment a descriptor handle offset
		mDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
	// but we cannot literally use it like a c++ pointer.
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; i++)
	{
		// first we get the n'th buffer in the swap chain and store it in the n'th
		// position of our ID3D12Resource array
		mHr = swapChain->GetBuffer(i, IID_PPV_ARGS(&mRenderTargets[i]));
		if (FAILED(mHr))
		{
			ASSERT(false);
			return false;
		}

		// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
		device->CreateRenderTargetView(mRenderTargets[i].Get(), nullptr, handle);

		// we increment the rtv handle by the rtv descriptor size we got above
		handle.Offset(1, mDescriptorSize);
	}

	return true;
}

std::wstring Descriptor::GetName() const
{
	return mName;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE Descriptor::GetHandle(int frameIndex) const
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, mDescriptorSize);
	return handle;
}

ComPtr<ID3D12Resource> Descriptor::GetRenderTarget(int pos) const
{
	return mRenderTargets[pos];
}
