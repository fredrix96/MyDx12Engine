#include "Dx12Engine.h"

Dx12Engine::Dx12Engine() :
	mIsInitialized(false), frameIndex(DEFAULT_VALUE), mMainWindow()
{
}

Dx12Engine::~Dx12Engine()
{
	// wait for the gpu to finish all frames
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; ++i)
	{
		frameIndex = i;
		waitForPreviousFrame();
	}

	// get swapchain out of full screen before exiting
	BOOL fs = false;
	if (mSwapChainMan.GetSwapChain()->GetFullscreenState(&fs, NULL) == (LRESULT)true)
	{
		mSwapChainMan.GetSwapChain()->SetFullscreenState(false, NULL);
	}
}

bool Dx12Engine::CreateMainWindow(HINSTANCE hInstance, int nShowCmd, int width, int height, bool fullscreen, LPCTSTR name, LPCTSTR title)
{
	if (mMainWindow.IsWindowCreated())
	{
		ASSERT(false);
		return false;
	}

	WindowManager tmpWindow(
		hInstance, nShowCmd,
		width, height,
		false,
		name, title);
	mMainWindow = tmpWindow;

	return true;
}

bool Dx12Engine::Initialize()
{
	if (mIsInitialized || !mMainWindow.IsWindowCreated())
	{
		ASSERT(false);
		return false;
	}

	bool isCreated = false;

	// -- Create the Device -- //

	isCreated = mDevMan.CreateDevice();
	ASSERT(isCreated);

	// -- Create a direct command queue -- //

	isCreated = mComQueMan.CreateDirectCommandQueue(mDevMan.GetDevice().Get());
	ASSERT(isCreated);

	// -- Create the Swap Chain (double/tripple buffering) -- //

	isCreated = mSwapChainMan.CreateSwapChain(mMainWindow, mDevMan.GetFactory().Get(),
		mComQueMan.GetDirectCommandQueue().Get(), NUM_OF_FRAME_BUFFERS);
	ASSERT(isCreated);

	frameIndex = mSwapChainMan.GetSwapChain()->GetCurrentBackBufferIndex();

	// -- Create the Back Buffers (render target views) Descriptor Heap -- //

	isCreated = mDescMan.CreateRTVDescriptorHeap(mDevMan.GetDevice().Get(), mSwapChainMan.GetSwapChain().Get(), first);
	ASSERT(isCreated);

	// -- Create the Depth/Stencil Buffer -- //

	isCreated = mDescMan.CreateDepthBuffer(mDevMan.GetDevice().Get(), mMainWindow.GetWidth(), mMainWindow.GetHeight(), first);
	ASSERT(isCreated);
	if (!isCreated) gWindowRunning = false;

	// -- Create the Command Allocators -- And -- Create a Command List --//

	isCreated = mComMan.CreateCommand(mDevMan.GetDevice().Get(), frameIndex, first);
	ASSERT(isCreated);

	// -- Create a Fence & Fence Event -- //

	isCreated = mFenceMan.CreateFences(mDevMan.GetDevice().Get());
	ASSERT(isCreated);

	// -- Create Root Signature -- //

	isCreated = mRootSignMan.CreateRootSignature(mDevMan.GetDevice().Get(), first);
	ASSERT(isCreated);

	// -- Create Shaders -- //

	// When debugging, we can compile the shader files at runtime.
	// but for release versions, we can compile the hlsl shaders
	// with fxc.exe to create .cso files, which contain the shader
	// bytecode. We can load the .cso files at runtime to get the
	// shader bytecode, which of course is faster than compiling
	// them at runtime

	isCreated = mGraphPipeMan.CreateGraphicsPipeline({ SHADER_TYPE::VERTEX, SHADER_TYPE::PIXEL }, first);
	ASSERT(isCreated);

	// -- Create Input Layout -- //

	// The input layout is used by the Input Assembler so that it knows
	// how to read the vertex data bound to it.

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	int inputLayoutSize = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);

	// -- Create a Pipeline State Object (PSO) -- //

	// In a real application, you will have many pso's. for each different shader
	// or different combinations of shaders, different blend states or different rasterizer states,
	// different topology types (point, line, triangle, patch), or a different number
	// of render targets you will need a pso

	// VS is the only required shader for a pso. You might be wondering when a case would be where
	// you only set the VS. It's possible that you have a pso that only outputs data with the stream
	// output, and not on a render target, which means you would not need anything after the stream
	// output.

	isCreated = mGraphPipeMan.CreatePSO(mDevMan.GetDevice().Get(),
		mRootSignMan.GetRootSignature(first).GetRootSignature().Get(),
		mSwapChainMan.GetSampleDesc(),
		{ inputLayout, inputLayoutSize },
		first);
	ASSERT(isCreated);

	// -- Create Vertex Buffer -- //

	// a quad
	Vertex vList[] = {
		// front face
		{ -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// right side face
		{  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{  0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// left side face
		{ -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// back face
		{  0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// top face
		{ -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// bottom face
		{  0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
	};

	int vBufferSize = sizeof(vList);

	isCreated = mD3DContextMan.CreateVertexBuffer(mDevMan.GetDevice().Get(), { vList, vBufferSize }, mComMan.GetCommand(first).GetCommandList().Get(), first);
	ASSERT(isCreated);

	// transition the vertex buffer data from copy destination state to vertex buffer state
	mComMan.GetCommand(first).GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		mD3DContextMan.GetVertexBuffer(first).GetBufferResource().Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	// -- Create Index Buffer -- //

	// a quad (2 triangles)
	DWORD iList[] = {
		// ffront face
		0, 1, 2, // first triangle
		0, 3, 1, // second triangle

		// left face
		4, 5, 6, // first triangle
		4, 7, 5, // second triangle

		// right face
		8, 9, 10, // first triangle
		8, 11, 9, // second triangle

		// back face
		12, 13, 14, // first triangle
		12, 15, 13, // second triangle

		// top face
		16, 17, 18, // first triangle
		16, 19, 17, // second triangle

		// bottom face
		20, 21, 22, // first triangle
		20, 23, 21, // second triangle
	};

	int iBufferSize = sizeof(iList);

	isCreated = mD3DContextMan.CreateIndexBuffer(mDevMan.GetDevice().Get(), { iList, iBufferSize }, mComMan.GetCommand(first).GetCommandList().Get(), first);
	ASSERT(isCreated);

	// transition the index buffer data from copy destination state to vertex buffer state
	mComMan.GetCommand(first).GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		mD3DContextMan.GetIndexBuffer(first).GetBufferResource().Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	// -- Create Constant Buffer Resource Heap -- //
	// We will update the constant buffer one or more times per frame, so we will use only an upload heap
	// unlike previously we used an upload heap to upload the vertex and index data, and then copied over
	// to a default heap. If you plan to use a resource for more than a couple frames, it is usually more
	// efficient to copy to a default heap where it stays on the gpu. In this case, our constant buffer
	// will be modified and uploaded at least once per frame, so we only use an upload heap

	// first we will create a resource heap (upload heap) for each frame for the cubes constant buffers
	// As you can see, we are allocating 64KB for each resource we create. Buffer resource heaps must be
	// an alignment of 64KB. We are creating 3 resources, one for each frame. Each constant buffer is 
	// only a 4x4 matrix of floats in this tutorial. So with a float being 4 bytes, we have 
	// 16 floats in one constant buffer, and we will store 2 constant buffers in each
	// heap, one for each cube, thats only 64x2 bits, or 128 bits we are using for each
	// resource, and each resource must be at least 64KB (65536 bits)

	isCreated = mD3DContextMan.CreateConstantBuffers(mDevMan.GetDevice().Get(), first);
	ASSERT(isCreated);

	// Now we execute the command list to upload the initial assets (triangle data)
	mComMan.GetCommand(first).GetCommandList()->Close();
	ComPtr<ID3D12CommandList> ppCommandLists[] = { mComMan.GetCommand(first).GetCommandList().Get() };
	mComQueMan.GetDirectCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists->GetAddressOf());

	// increment the fence value now, otherwise the buffer might not be uploaded by the time we start drawing
	mFenceMan.IncrementFenceValue(frameIndex);
	HRESULT hr = mComQueMan.GetDirectCommandQueue()->Signal(mFenceMan.GetFence(frameIndex).Get(), mFenceMan.GetFenceValue(frameIndex));
	if (FAILED(hr))
	{
		gWindowRunning = false;
	}

	// -- Camera Set Up -- //

	// Fill out the Viewport
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)mMainWindow.GetWidth();
	viewport.Height = (float)mMainWindow.GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Fill out a scissor rect
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = mMainWindow.GetWidth();
	scissorRect.bottom = mMainWindow.GetHeight();

	// build projection and view matrix
	XMMATRIX tmpMat = XMMatrixPerspectiveFovLH(45.0f * (3.14f / 180.0f), (float)mMainWindow.GetWidth() / (float)mMainWindow.GetHeight(), 0.1f, 1000.0f);
	XMStoreFloat4x4(&cameraProjMat, tmpMat);

	// set starting camera state
	cameraPosition = XMFLOAT4(0.0f, 2.0f, -4.0f, 0.0f);
	cameraTarget = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	cameraUp = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	// build view matrix
	XMVECTOR cPos = XMLoadFloat4(&cameraPosition);
	XMVECTOR cTarg = XMLoadFloat4(&cameraTarget);
	XMVECTOR cUp = XMLoadFloat4(&cameraUp);
	tmpMat = XMMatrixLookAtLH(cPos, cTarg, cUp);
	XMStoreFloat4x4(&cameraViewMat, tmpMat);

	// -- Object Placement -- //

	// set starting cubes position
	// first cube
	cube1Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); // set cube 1's position
	XMVECTOR posVec = XMLoadFloat4(&cube1Position); // create xmvector for cube1's position

	tmpMat = XMMatrixTranslationFromVector(posVec); // create translation matrix from cube1's position vector
	XMStoreFloat4x4(&cube1RotMat, XMMatrixIdentity()); // initialize cube1's rotation matrix to identity matrix
	XMStoreFloat4x4(&cube1WorldMat, tmpMat); // store cube1's world matrix

	// second cube
	cube2PositionOffset = XMFLOAT4(1.5f, 0.0f, 0.0f, 0.0f);
	posVec = XMLoadFloat4(&cube2PositionOffset) + XMLoadFloat4(&cube1Position); // create xmvector for cube2's position
																				// we are rotating around cube1 here, so add cube2's position to cube1

	tmpMat = XMMatrixTranslationFromVector(posVec); // create translation matrix from cube2's position offset vector
	XMStoreFloat4x4(&cube2RotMat, XMMatrixIdentity()); // initialize cube2's rotation matrix to identity matrix
	XMStoreFloat4x4(&cube2WorldMat, tmpMat); // store cube2's world matrix

	return true;
}

void Dx12Engine::UpdatePipeline()
{
	HRESULT hr;

	// We have to wait for the gpu to finish with the command allocator before we reset it
	waitForPreviousFrame();

	// we can only reset an allocator once the gpu is done with it
	// resetting an allocator frees the memory that the command list was stored in
	hr = mComMan.GetCommand(first).GetCommandAllocator(frameIndex)->Reset();
	if (FAILED(hr))
	{
		gWindowRunning = false;
	}

	// reset the command list. by resetting the command list we are putting it into
	// a recording state so we can start recording commands into the command allocator.
	// the command allocator that we reference here may have multiple command lists
	// associated with it, but only one can be recording at any time. Make sure
	// that any other command lists associated to this command allocator are in
	// the closed state (not recording).
	// Here you will pass an initial pipeline state object as the second parameter,
	// but in this tutorial we are only clearing the rtv, and do not actually need
	// anything but an initial default pipeline, which is what we get by setting
	// the second parameter to NULL
	hr = mComMan.GetCommand(first).GetCommandList()->Reset(
		mComMan.GetCommand(first).GetCommandAllocator(frameIndex).Get(),
		mGraphPipeMan.GetPSO(first).GetPSO().Get());
	if (FAILED(hr))
	{
		gWindowRunning = false;
	}

	// here we start recording commands into the commandList (which all the commands will be stored in the commandAllocator)

	// transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here
	mComMan.GetCommand(first).GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		mDescMan.GetRTVDescriptor(first).GetRenderTarget(frameIndex).Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	// here we again get the handle to our current render target view so we can set it as the render target in the output merger stage of the pipeline
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = mDescMan.GetRTVDescriptor(first).GetHandle(frameIndex);

	// get a handle to the depth/stencil buffer
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDescMan.GetDepthBuffer(first).GetHandle();

	// set the render target for the output merger stage (the output of the pipeline)
	mComMan.GetCommand(first).GetCommandList()->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	// Clear the render target by using the ClearRenderTargetView command
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	mComMan.GetCommand(first).GetCommandList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// clear the depth/stencil buffer
	mComMan.GetCommand(first).GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// set root signature
	mComMan.GetCommand(first).GetCommandList()->SetGraphicsRootSignature(mRootSignMan.GetRootSignature(first).GetRootSignature().Get()); // set the root signature

	// draw triangle
	mComMan.GetCommand(first).GetCommandList()->RSSetViewports(1, &viewport); // set the viewports
	mComMan.GetCommand(first).GetCommandList()->RSSetScissorRects(1, &scissorRect); // set the scissor rects
	mComMan.GetCommand(first).GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // set the primitive topology
	mComMan.GetCommand(first).GetCommandList()->IASetVertexBuffers(0, 1, &mD3DContextMan.GetVertexBuffer(first).GetVertexBufferView()); // set the vertex buffer (using the vertex buffer view)
	mComMan.GetCommand(first).GetCommandList()->IASetIndexBuffer(&mD3DContextMan.GetIndexBuffer(first).GetIndexBufferView());

	// first cube

	// set cube1's constant buffer
	mComMan.GetCommand(first).GetCommandList()->SetGraphicsRootConstantBufferView(0,
		mD3DContextMan.GetConstantBuffer(first).GetBufferResource(frameIndex)->GetGPUVirtualAddress());

	// draw first cube
	mComMan.GetCommand(first).GetCommandList()->DrawIndexedInstanced(mD3DContextMan.GetIndexBuffer(first).GetNumOfIndices(), 1, 0, 0, 0);

	// second cube

	// set cube2's constant buffer. You can see we are adding the size of ConstantBufferPerObject to the constant buffer
	// resource heaps address. This is because cube1's constant buffer is stored at the beginning of the resource heap, while
	// cube2's constant buffer data is stored after (256 bits from the start of the heap).
	mComMan.GetCommand(first).GetCommandList()->SetGraphicsRootConstantBufferView(0,
		mD3DContextMan.GetConstantBuffer(first).GetBufferResource(frameIndex)->GetGPUVirtualAddress() + ConstantBufferPerObjectAlignedSize);

	// draw second cube
	mComMan.GetCommand(first).GetCommandList()->DrawIndexedInstanced(mD3DContextMan.GetIndexBuffer(first).GetNumOfIndices(), 1, 0, 0, 0);

	// transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
	// warning if present is called on the render target when it's not in the present state
	mComMan.GetCommand(first).GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		mDescMan.GetRTVDescriptor(first).GetRenderTarget(frameIndex).Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	hr = mComMan.GetCommand(first).GetCommandList()->Close();
	if (FAILED(hr))
	{
		gWindowRunning = false;
	}
}

void Dx12Engine::waitForPreviousFrame()
{
	HRESULT hr;

	// swap the current rtv buffer index so we draw on the correct buffer
	frameIndex = mSwapChainMan.GetSwapChain()->GetCurrentBackBufferIndex();

	// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
	// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
	if (mFenceMan.GetFence(frameIndex)->GetCompletedValue() < mFenceMan.GetFenceValue(frameIndex))
	{
		// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
		hr = mFenceMan.GetFence(frameIndex)->SetEventOnCompletion(mFenceMan.GetFenceValue(frameIndex), mFenceMan.GetFenceEvent());
		if (FAILED(hr))
		{
			gWindowRunning = false;
		}

		// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
		// has reached "fenceValue", we know the command queue has finished executing
		WaitForSingleObject(mFenceMan.GetFenceEvent(), INFINITE);
	}

	// increment fenceValue for next frame
	mFenceMan.IncrementFenceValue(frameIndex);
}

void Dx12Engine::Render()
{
	HRESULT hr;

	UpdatePipeline(); // update the pipeline by sending commands to the commandqueue

	// create an array of command lists (only one command list here)
	ComPtr<ID3D12CommandList> ppCommandLists[] = { mComMan.GetCommand(first).GetCommandList().Get() };

	// execute the array of command lists
	ComPtr<ID3D12CommandQueue> commandQueue = mComQueMan.GetDirectCommandQueue();
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists->GetAddressOf());

	// this command goes in at the end of our command queue. we will know when our command queue 
	// has finished because the fence value will be set to "fenceValue" from the GPU since the command
	// queue is being executed on the GPU
	hr = commandQueue->Signal(mFenceMan.GetFence(frameIndex).Get(), mFenceMan.GetFenceValue(frameIndex));
	if (FAILED(hr))
	{
		gWindowRunning = false;
	}

	// present the current backbuffer
	hr = mSwapChainMan.GetSwapChain()->Present(0, 0);
	if (FAILED(hr))
	{
		gWindowRunning = false;
	}
}

void Dx12Engine::EnableDebugLayer()
{
#if defined(_DEBUG)

	// dump memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer.
	ComPtr<ID3D12Debug> debugInterface;
	//ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface));
	debugInterface->EnableDebugLayer();
#endif
}

WindowManager Dx12Engine::GetMainWindow() const
{
	return mMainWindow;
}

void Dx12Engine::UpdateGameLogic()
{
	// create rotation matrices
	XMMATRIX rotXMat = XMMatrixRotationX(0.0002f);
	XMMATRIX rotYMat = XMMatrixRotationY(0.0004f);
	XMMATRIX rotZMat = XMMatrixRotationZ(0.0006f);

	// add rotation to cube1's rotation matrix and store it
	XMMATRIX rotMat = XMLoadFloat4x4(&cube1RotMat) * rotXMat * rotYMat * rotZMat;
	XMStoreFloat4x4(&cube1RotMat, rotMat);

	// create translation matrix for cube 1 from cube 1's position vector
	XMMATRIX translationMat = XMMatrixTranslationFromVector(XMLoadFloat4(&cube1Position));

	// create cube1's world matrix by first rotating the cube, then positioning the rotated cube
	XMMATRIX worldMat = rotMat * translationMat;

	// store cube1's world matrix
	XMStoreFloat4x4(&cube1WorldMat, worldMat);

	// update constant buffer for cube1
	// create the wvp matrix and store in constant buffer
	XMMATRIX viewMat = XMLoadFloat4x4(&cameraViewMat); // load view matrix
	XMMATRIX projMat = XMLoadFloat4x4(&cameraProjMat); // load projection matrix
	XMMATRIX wvpMat = XMLoadFloat4x4(&cube1WorldMat) * viewMat * projMat; // create wvp matrix
	XMMATRIX transposed = XMMatrixTranspose(wvpMat); // must transpose wvp matrix for the gpu
	mD3DContextMan.GetConstantBuffer(first).StoreValues(transposed);  // store transposed wvp matrix in constant buffer

	// copy our ConstantBuffer instance to the mapped constant buffer resource
	memcpy(mD3DContextMan.GetConstantBuffer(first).GetCBVGPUAddress(frameIndex),
		mD3DContextMan.GetConstantBuffer(first).GetCBPerObject(),
		sizeof(mD3DContextMan.GetConstantBuffer(first).GetCBPerObject()));
	int size = sizeof(mD3DContextMan.GetConstantBuffer(first).GetCBPerObject());

	// now do cube2's world matrix
	// create rotation matrices for cube2
	rotXMat = XMMatrixRotationX(0.0006f);
	rotYMat = XMMatrixRotationY(0.0004f);
	rotZMat = XMMatrixRotationZ(0.0002f);

	// add rotation to cube2's rotation matrix and store it
	rotMat = rotZMat * (XMLoadFloat4x4(&cube2RotMat) * (rotXMat * rotYMat));
	XMStoreFloat4x4(&cube2RotMat, rotMat);

	// create translation matrix for cube 2 to offset it from cube 1 (its position relative to cube1
	XMMATRIX translationOffsetMat = XMMatrixTranslationFromVector(XMLoadFloat4(&cube2PositionOffset));

	// we want cube 2 to be half the size of cube 1, so we scale it by .5 in all dimensions
	XMMATRIX scaleMat = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// reuse worldMat. 
	// first we scale cube2. scaling happens relative to point 0,0,0, so you will almost always want to scale first
	// then we translate it. 
	// then we rotate it. rotation always rotates around point 0,0,0
	// finally we move it to cube 1's position, which will cause it to rotate around cube 1
	worldMat = scaleMat * translationOffsetMat * rotMat * translationMat;

	wvpMat = XMLoadFloat4x4(&cube2WorldMat) * viewMat * projMat; // create wvp matrix
	transposed = XMMatrixTranspose(wvpMat); // must transpose wvp matrix for the gpu
	mD3DContextMan.GetConstantBuffer(first).StoreValues(transposed); // store transposed wvp matrix in constant buffer

	// copy our ConstantBuffer instance to the mapped constant buffer resource
	memcpy(mD3DContextMan.GetConstantBuffer(first).GetCBVGPUAddress(frameIndex) + ConstantBufferPerObjectAlignedSize, 
		mD3DContextMan.GetConstantBuffer(first).GetCBPerObject(),
		sizeof(mD3DContextMan.GetConstantBuffer(first).GetCBPerObject()));

	// store cube2's world matrix
	XMStoreFloat4x4(&cube2WorldMat, worldMat);
}