#include "command.h"

Command::Command() :
	mCommandAllocator{ NULL }, mCommandList(NULL), mNameAlloc(), mNameList(), mHr(NULL)
{
}

Command::~Command()
{
}

bool Command::CreateCommandAllocators(ID3D12Device* device, std::wstring name)
{
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; i++)
	{
		if (mCommandAllocator[i] != NULL)
		{
			ASSERT(false);
			return false;
		}

		mHr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator[i]));
		if (FAILED(mHr))
		{
			return false;
		}

		mNameAlloc = L"commandAllocator_" + name + L"_" + std::to_wstring(i);
		mCommandAllocator[i]->SetName(mNameAlloc.c_str());
	}

	return true;
}

bool Command::CreateCommandList(ID3D12Device* device, int frameIndex, std::wstring name)
{
	// create the command list with the first allocator
	mHr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator[frameIndex].Get(), NULL, IID_PPV_ARGS(mCommandList.GetAddressOf()));
	if (FAILED(mHr))
	{
		return false;
	}

	mNameList = L"commandList_" + name;
	mCommandList->SetName(mNameList.c_str());

	return true;
}

ComPtr<ID3D12CommandAllocator> Command::GetCommandAllocator(int pos) const
{
	return mCommandAllocator[pos];
}

ComPtr<ID3D12GraphicsCommandList> Command::GetCommandList() const
{
	return mCommandList;
}
