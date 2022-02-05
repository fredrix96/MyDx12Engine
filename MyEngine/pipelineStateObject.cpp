#include "pipelineStateObject.h"

PipelineStateObject::PipelineStateObject() :
	mPipelineStateObject(NULL), mInputLayoutDesc({}), mInputLayout(NULL), mPsoDesc({}), mHr(NULL)
{
}

PipelineStateObject::~PipelineStateObject()
{
}

bool PipelineStateObject::CreatePSO(ID3D12Device* device,
	ID3D12RootSignature* rootSignature,
	DXGI_SAMPLE_DESC sampleDesc,
	std::map<SHADER_TYPE, Shader> shaderMap,
	std::pair<D3D12_INPUT_ELEMENT_DESC*, int> inputLayout,
	std::wstring name)
{
	if (mPipelineStateObject != NULL)
	{
		ASSERT(false);
		return false;
	}

	// fill out an input layout description structure
	mInputLayoutDesc.NumElements = inputLayout.second;
	mInputLayoutDesc.pInputElementDescs = inputLayout.first;

	mPsoDesc.InputLayout = mInputLayoutDesc; // the structure describing our input layout
	mPsoDesc.pRootSignature = rootSignature; // the root signature that describes the input data this pso needs
	mPsoDesc.VS = shaderMap.at(SHADER_TYPE::VERTEX).GetShaderByteCode(); // structure describing where to find the vertex shader bytecode and how large it is
	mPsoDesc.PS = shaderMap.at(SHADER_TYPE::PIXEL).GetShaderByteCode(); // same as VS but for pixel shader
	mPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
	mPsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the render target
	mPsoDesc.SampleDesc = sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
	mPsoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
	mPsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
	mPsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // a default blent state.
	mPsoDesc.NumRenderTargets = 1; // we are only binding one render target
	mPsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // a default depth stencil state

	// create the pso
	mHr = device->CreateGraphicsPipelineState(&mPsoDesc, IID_PPV_ARGS(&mPipelineStateObject));
	if (FAILED(mHr))
	{
		return false;
	}

	mName = L"pso_" + name;
	mPipelineStateObject->SetName(mName.c_str());

	return true;
}

ComPtr<ID3D12PipelineState> PipelineStateObject::GetPSO() const
{
	return mPipelineStateObject;
}
