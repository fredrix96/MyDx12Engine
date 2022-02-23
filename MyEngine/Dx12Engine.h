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
