#include "shaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::CreateGraphicsPipeline(std::list<SHADER_TYPE> types, std::wstring name)
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

	mGraphicsPipeline.emplace(name, tmpMap);

	return isCreated;
}

std::map<SHADER_TYPE, Shader> ShaderManager::GetGraphicsPipeline(std::wstring name) const
{
	return mGraphicsPipeline.at(name);
}
