#pragma once

#include "renderTargetView.h"
#include "depthBuffer.h"

class DescriptorManager
{
private:
	std::map<std::wstring, RenderTargetView> mRTVMap;
	std::map<std::wstring, DepthBuffer> mDepthBufferMap;

	HRESULT mHr;
public:

	DescriptorManager();
	~DescriptorManager();
	
	bool CreateRTVDescriptorHeap(ID3D12Device* device, IDXGISwapChain3* swapChain, std::wstring name);
	bool CreateDepthBuffer(ID3D12Device* device, int windowWidth, int windowHeight, std::wstring name);

	RenderTargetView & GetRTVDescriptor(std::wstring name);
	DepthBuffer & GetDepthBuffer(std::wstring name);
};
