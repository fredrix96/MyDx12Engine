#pragma once

#include "shader.h"

class ShaderManager
{
private:
	std::map<std::wstring, std::map<SHADER_TYPE, Shader>> mGraphicsPipeline;

public:

	ShaderManager();
	~ShaderManager();

	bool CreateGraphicsPipeline(std::list<SHADER_TYPE> types, std::wstring name);

	std::map<SHADER_TYPE, Shader> GetGraphicsPipeline(std::wstring name) const;
};
