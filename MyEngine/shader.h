#pragma once

#include "stdafx.h"

class Shader
{
private:
	ComPtr<ID3DBlob> mShader; // d3d blob for holding shader bytecode
	ComPtr<ID3DBlob> mErrorBuff; // a buffer holding the error data if any

	D3D12_SHADER_BYTECODE mShaderBytecode;

	HRESULT mHr;
public:

	Shader();
	~Shader();

	bool CreateShader(SHADER_TYPE type, std::wstring name);

	D3D12_SHADER_BYTECODE GetShaderByteCode() const;
};
