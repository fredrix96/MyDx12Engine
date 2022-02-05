#include "graphicsPipelineManager.h"

GraphicsPipelineManager::GraphicsPipelineManager()
{
}

GraphicsPipelineManager::~GraphicsPipelineManager()
{
}

bool GraphicsPipelineManager::CreateGraphicsPipeline(std::list<SHADER_TYPE> types, std::wstring name)
{
	bool isCreated = false;

	Shader tmpVertexShader;
	Shader tmpPixelShader;

	std::map<SHADER_TYPE, Shader> tmpMap;

	for (auto type : types)
	{
		Shader shader;
		switch (type)
		{
		case SHADER_TYPE::VERTEX:
			isCreated = tmpVertexShader.CreateShader(type, name);
			if (isCreated)
			{
				tmpMap.emplace(type, tmpVertexShader);
			}
			else
			{
				ASSERT(false);
				return false;
			}
			break;
		case SHADER_TYPE::PIXEL:
			isCreated = tmpPixelShader.CreateShader(type, name);
			if (isCreated)
			{
				tmpMap.emplace(type, tmpPixelShader);
			}
			else
			{
				ASSERT(false);
				return false;
			}
			break;
		default:
			ASSERT(false);
			return false;
		}
	}

	mGraphicsPipelineMap.emplace(name, tmpMap);

	return isCreated;
}

bool GraphicsPipelineManager::CreatePSO(ID3D12Device* device,
	ID3D12RootSignature* rootSignature,
	DXGI_SAMPLE_DESC sampleDesc,
	std::pair<D3D12_INPUT_ELEMENT_DESC*, int> inputLayout,
	std::wstring name)
{
	PipelineStateObject pso;
	bool isCreated = pso.CreatePSO(device, rootSignature, sampleDesc, mGraphicsPipelineMap.at(name), inputLayout, name); //TODO: Make private?

	if (isCreated)
	{
		mPSOMap.emplace(name, pso);
	}

	return isCreated;
}

std::map<SHADER_TYPE, Shader> GraphicsPipelineManager::GetGraphicsPipeline(std::wstring name) const
{
	return mGraphicsPipelineMap.at(name);
}

PipelineStateObject GraphicsPipelineManager::GetPSO(std::wstring name) const
{
	return mPSOMap.at(name);
}
