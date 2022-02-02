#include "deviceManager.h"

DeviceManager::DeviceManager():
	mDevice(NULL), mFactory(NULL), mAdapter(NULL), mHr(NULL)
{
}

DeviceManager::~DeviceManager()
{
}

bool DeviceManager::CreateDevice()
{
	if (mDevice != NULL || mFactory != NULL) return false;

	mHr = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));

	if (FAILED(mHr)) return false;

	int adapterIndex = 0; // we'll start looking for directx 12 compatible graphics devices starting at index 0
	bool adapterFound = false; // set this to true when a good one was found

	// find first hardware gpu that supports d3d 12
	while (mFactory->EnumAdapters1(adapterIndex, &mAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		mAdapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// we dont want a software device
			continue;
		}

		// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
		mHr = D3D12CreateDevice(mAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(mHr))
		{
			adapterFound = true;
			break;
		}

		adapterIndex++;
	}

	if (!adapterFound)
	{
		return false;
	}

	// Create the device
	mHr = D3D12CreateDevice(
		mAdapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&mDevice)
	);
	if (FAILED(mHr))
	{
		return false;
	}

	return true;
}

ComPtr<ID3D12Device> const DeviceManager::GetDevice() const
{
	return mDevice;
}

ComPtr<IDXGIFactory4> const DeviceManager::GetFactory() const
{
	return mFactory;
}
