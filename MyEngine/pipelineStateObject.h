#pragma once

#include "shader.h"

class PipelineStateObject
{
private:
	ComPtr<ID3D12PipelineState> mPipelineStateObject; // pso containing a pipeline state
	D3D12_INPUT_ELEMENT_DESC* mInputLayout;
	D3D12_INPUT_LAYOUT_DESC mInputLayoutDesc;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC mPsoDesc; // a structure to define a pso

	std::wstring mName;

	HRESULT mHr;

public:
	PipelineStateObject();
	~PipelineStateObject();

	bool CreatePSO(ID3D12Device* device,
		ID3D12RootSignature* rootSignature,
		DXGI_SAMPLE_DESC sampleDesc,
		std::map<SHADER_TYPE, Shader> shaderMap,
		std::pair<D3D12_INPUT_ELEMENT_DESC*, int> inputLayout,
		std::wstring name);

	ComPtr<ID3D12PipelineState> GetPSO() const;
};