#pragma once

#include "stdafx.h"

class RootSignature
{
private:
	ComPtr<ID3D12RootSignature> mRootSignature; // root signature defines data shaders will access
	ComPtr<ID3DBlob> mSignature;

	D3D12_ROOT_DESCRIPTOR mRootDescriptor;
	D3D12_ROOT_PARAMETER mRootParameters[1]; // only one parameter right now
	CD3DX12_ROOT_SIGNATURE_DESC mRootSignatureDesc;

	std::wstring mName;
	HRESULT mHr;
public:

	RootSignature();
	~RootSignature();

	bool CreateRootSignature(ID3D12Device* device, std::wstring name);

	ComPtr<ID3D12RootSignature> GetRootSignature() const;
};