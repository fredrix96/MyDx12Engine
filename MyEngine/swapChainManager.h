#pragma once

#include "stdafx.h"

#pragma region FORWARD_DECLARATIONS
	class WindowManager;
#pragma endregion

class SwapChainManager
{
private:
	ComPtr<IDXGISwapChain3> mSwapChain; // swapchain used to switch between render targets
	DXGI_MODE_DESC mBackBufferDesc; // this is to describe our display mode
	DXGI_SAMPLE_DESC mSampleDesc; // describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
	DXGI_SWAP_CHAIN_DESC mSwapChainDesc;

public:
	SwapChainManager();
	~SwapChainManager();

	bool CreateSwapChain(WindowManager& window,
		IDXGIFactory4* device,
		ID3D12CommandQueue* commandQueue,
		int frameBufferCount);
	ComPtr<IDXGISwapChain3> const GetSwapChain() const;
	DXGI_MODE_DESC const GetBackBufferDesc() const;
	DXGI_SAMPLE_DESC const GetSampleDesc() const;
	DXGI_SWAP_CHAIN_DESC const GetSwapChainDesc() const;
};
