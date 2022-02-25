#pragma once

#include "rootSignature.h"

class RootSignatureManager
{
private:
	std::map<std::wstring, RootSignature> mRootSignatures;

public:

	RootSignatureManager();
	~RootSignatureManager();

	bool CreateRootSignature(ID3D12Device* device, std::wstring name);

	RootSignature & GetRootSignature(std::wstring name);
};