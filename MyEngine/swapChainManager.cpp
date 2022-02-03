#include "swapChainManager.h"
#include "windowManager.h"

SwapChainManager::SwapChainManager() :
	mSwapChain(NULL), mBackBufferDesc({}), mSampleDesc({}), mSwapChainDesc({})
{
}

SwapChainManager::~SwapChainManager()
{
}

bool SwapChainManager::CreateSwapChain(WindowManager& window,
	IDXGIFactory4* factory,
	ID3D12CommandQueue* commandQueue,
	int frameBufferCount)
{
	if (mSwapChain != NULL) return false;

	mBackBufferDesc.Width = window.GetWidth(); // buffer width
	mBackBufferDesc.Height = window.GetHeight(); // buffer height
	mBackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)

	mSampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

	// Describe and create the swap chain.
	mSwapChainDesc.BufferCount = frameBufferCount; // number of buffers we have
	mSwapChainDesc.BufferDesc = mBackBufferDesc; // our back buffer description
	mSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
	mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
	mSwapChainDesc.OutputWindow = window.GetHWND(); // handle to our window
	mSwapChainDesc.SampleDesc = mSampleDesc; // our multi-sampling description
	mSwapChainDesc.Windowed = !window.IsFullscreen(); // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

	IDXGISwapChain* tempSwapChain;

	factory->CreateSwapChain(
		commandQueue, // the queue will be flushed once the swap chain is created
		&mSwapChainDesc, // give it the swap chain description we created above
		&tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
	);

	mSwapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

	if (mSwapChain != NULL)
	{
		return true;
	}
	return false;
}

ComPtr<IDXGISwapChain3> const SwapChainManager::GetSwapChain() const
{
	return mSwapChain;
}

DXGI_MODE_DESC const SwapChainManager::GetBackBufferDesc() const
{
	return mBackBufferDesc;
}

DXGI_SAMPLE_DESC const SwapChainManager::GetSampleDesc() const
{
	return mSampleDesc;
}

DXGI_SWAP_CHAIN_DESC const SwapChainManager::GetSwapChainDesc() const
{
	return mSwapChainDesc;
}
