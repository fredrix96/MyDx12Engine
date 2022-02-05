#include "shader.h"

Shader::Shader() :
	mShader(NULL), mShaderBytecode({}), mErrorBuff(NULL), mHr(NULL)
{
}

Shader::~Shader()
{
}

bool Shader::CreateShader(SHADER_TYPE type, std::wstring name)
{
	if (mShader != NULL)
	{
		ASSERT(false);
		return false;
	}

	// compile shader
	switch (type)
	{
	case SHADER_TYPE::VERTEX:
		mHr = D3DCompileFromFile((L"VertexShader_" + name + L".hlsl").c_str(),
			nullptr,
			nullptr,
			"main",
			("vs_" + SHADER_LEVEL).c_str(),
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&mShader,
			&mErrorBuff);
		break;
	case SHADER_TYPE::PIXEL:
		mHr = D3DCompileFromFile((L"PixelShader_" + name + L".hlsl").c_str(),
			nullptr,
			nullptr,
			"main",
			("ps_" + SHADER_LEVEL).c_str(),
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&mShader,
			&mErrorBuff);
		break;
	default:
		break;
	}

	if (FAILED(mHr))
	{
		OutputDebugStringA((char*)mErrorBuff->GetBufferPointer());
		return false;
	}

	// fill out a shader bytecode structure, which is basically just a pointer
	// to the shader bytecode and the size of the shader bytecode
	mShaderBytecode.BytecodeLength = mShader->GetBufferSize();
	mShaderBytecode.pShaderBytecode = mShader->GetBufferPointer();

	return true;
}

D3D12_SHADER_BYTECODE Shader::GetShaderByteCode() const
{
	return mShaderBytecode;
}
