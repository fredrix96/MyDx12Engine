#include "rootSignature.h"

RootSignature::RootSignature() :
	mRootSignature(NULL), mSignature(NULL), mRootDescriptor({}), mRootParameters(), mRootSignatureDesc(), mHr(NULL)
{
}

RootSignature::~RootSignature()
{
}

bool RootSignature::CreateRootSignature(ID3D12Device* device, std::wstring name)
{
	if (mRootSignature != NULL)
	{
		ASSERT(false);
		return false;
	}

	// create a root descriptor, which explains where to find the data for this root parameter
	mRootDescriptor.RegisterSpace = 0;
	mRootDescriptor.ShaderRegister = 0;

	// create a root parameter and fill it out
	mRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // this is a constant buffer view root descriptor
	mRootParameters[0].Descriptor = mRootDescriptor; // this is the root descriptor for this root parameter
	mRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // our pixel shader will be the only shader accessing this parameter for now

	mRootSignatureDesc.Init(_countof(mRootParameters), // we have 1 root parameter
		mRootParameters, // a pointer to the beginning of our root parameters array
		0,
		nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // we can deny shader stages here for better performance
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS);

	mHr = D3D12SerializeRootSignature(&mRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &mSignature, nullptr);
	if (FAILED(mHr))
	{
		return false;
	}

	mHr = device->CreateRootSignature(0, mSignature->GetBufferPointer(), mSignature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
	if (FAILED(mHr))
	{
		return false;
	}

	mName = L"rootSignature_" + name;
	mRootSignature->SetName(mName.c_str());

	return true;
}

ComPtr<ID3D12RootSignature> RootSignature::GetRootSignature() const
{
	return mRootSignature;
}
