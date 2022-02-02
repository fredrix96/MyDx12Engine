#pragma once

#include "stdafx.h"

class DeviceManager
{
private: 
	ComPtr<ID3D12Device> mDevice;
	ComPtr<IDXGIFactory4> mFactory;
	ComPtr<IDXGIAdapter1> mAdapter; // adapters are the graphics card (this includes the embedded graphics on the motherboard)

	HRESULT mHr;
public:
	DeviceManager();
	~DeviceManager();

	bool CreateDevice();

	ComPtr<ID3D12Device> const GetDevice() const;
	ComPtr<IDXGIFactory4> const GetFactory() const;
};
