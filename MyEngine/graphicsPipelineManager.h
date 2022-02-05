#pragma once

#include "pipelineStateObject.h"

class GraphicsPipelineManager
{
private:
	std::map<std::wstring, std::map<SHADER_TYPE, Shader>> mGraphicsPipelineMap;
	std::map<std::wstring, PipelineStateObject> mPSOMap;

public:

	GraphicsPipelineManager();
	~GraphicsPipelineManager();

	bool CreateGraphicsPipeline(std::list<SHADER_TYPE> types, std::wstring name);
	bool CreatePSO(ID3D12Device* device,
		ID3D12RootSignature* rootSignature,
		DXGI_SAMPLE_DESC sampleDesc,
		std::pair<D3D12_INPUT_ELEMENT_DESC*, int> inputLayout,
		std::wstring name);

	std::map<SHADER_TYPE, Shader> GetGraphicsPipeline(std::wstring name) const;

	PipelineStateObject GetPSO(std::wstring name) const;
};
