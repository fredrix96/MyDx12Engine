#pragma once

#include "windowManager.h"

#include "deviceManager.h"
#include "commandQueueManager.h"
#include "swapChainManager.h"
#include "descriptorManager.h"
#include "commandManager.h"
#include "fenceManager.h"
#include "rootSignatureManager.h"
#include "graphicsPipelineManager.h"
#include "direct3DContextManager.h"

class Dx12Engine
{
private:
	WindowManager mMainWindow;

	DeviceManager mDevMan;
	CommandQueueManager mComQueMan;
	SwapChainManager mSwapChainMan;
	DescriptorManager mDescMan;
	CommandManager mComMan;
	FenceManager mFenceMan;
	RootSignatureManager mRootSignMan;
	GraphicsPipelineManager mGraphPipeMan;
	Direct3DContextManager mD3DContextMan;

	int frameIndex; // current rtv we are on
	bool mIsInitialized;

	//tmp first creation name
	std::wstring first = L"first";

	void waitForPreviousFrame(); // wait until gpu is finished with command list

#pragma region globals

	// direct3d stuff

	D3D12_VIEWPORT viewport; // area that output from rasterizer will be stretched to.

	D3D12_RECT scissorRect; // the area to draw in. pixels outside that area will not be drawn onto

	ComPtr<ID3D12Resource> depthStencilBuffer; // This is the memory for our depth buffer. it will also be used for a stencil buffer in a later tutorial
	ComPtr<ID3D12DescriptorHeap> dsDescriptorHeap; // This is a heap for our depth/stencil buffer descriptor

	// this is the structure of our constant buffer.
	struct ConstantBufferPerObject {
		XMFLOAT4X4 wvpMat;
	};

	// Constant buffers must be 256-byte aligned which has to do with constant reads on the GPU.
	// We are only able to read at 256 byte intervals from the start of a resource heap, so we will
	// make sure that we add padding between the two constant buffers in the heap (one for cube1 and one for cube2)
	// Another way to do this would be to add a float array in the constant buffer structure for padding. In this case
	// we would need to add a float padding[50]; after the wvpMat variable. This would align our structure to 256 bytes (4 bytes per float)
	// The reason i didn't go with this way, was because there would actually be wasted cpu cycles when memcpy our constant
	// buffer data to the gpu virtual address. currently we memcpy the size of our structure, which is 16 bytes here, but if we
	// were to add the padding array, we would memcpy 64 bytes if we memcpy the size of our structure, which is 50 wasted bytes
	// being copied.
	int ConstantBufferPerObjectAlignedSize = (sizeof(ConstantBufferPerObject) + 255) & ~255;

	ConstantBufferPerObject cbPerObject; // this is the constant buffer data we will send to the gpu 
											// (which will be placed in the resource we created above)

	ComPtr<ID3D12Resource> constantBufferUploadHeaps[NUM_OF_FRAME_BUFFERS]; // this is the memory on the gpu where constant buffers for each frame will be placed

	UINT8* cbvGPUAddress[NUM_OF_FRAME_BUFFERS]; // this is a pointer to each of the constant buffer resource heaps

	XMFLOAT4X4 cameraProjMat; // this will store our projection matrix
	XMFLOAT4X4 cameraViewMat; // this will store our view matrix

	XMFLOAT4 cameraPosition; // this is our cameras position vector
	XMFLOAT4 cameraTarget; // a vector describing the point in space our camera is looking at
	XMFLOAT4 cameraUp; // the worlds up vector

	XMFLOAT4X4 cube1WorldMat; // our first cubes world matrix (transformation matrix)
	XMFLOAT4X4 cube1RotMat; // this will keep track of our rotation for the first cube
	XMFLOAT4 cube1Position; // our first cubes position in space

	XMFLOAT4X4 cube2WorldMat; // our first cubes world matrix (transformation matrix)
	XMFLOAT4X4 cube2RotMat; // this will keep track of our rotation for the second cube
	XMFLOAT4 cube2PositionOffset; // our second cube will rotate around the first cube, so this is the position offset from the first cube

#pragma endregion

public:
	Dx12Engine();
	~Dx12Engine(); // release com ojects and clean up memory

	bool CreateMainWindow(HINSTANCE hInstance, int nShowCmd, int width, int height, bool fullscreen, LPCTSTR name, LPCTSTR title);
	bool Initialize();
	
	void UpdatePipeline(); // update the direct3d pipeline (update command lists)
	void Render(); // execute the command list

	void EnableDebugLayer(); // enable debug layer

	WindowManager GetMainWindow() const;

	//tmp
	void UpdateGameLogic();
};
