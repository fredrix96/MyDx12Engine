#include "rootSignatureManager.h"

RootSignatureManager::RootSignatureManager()
{
}

RootSignatureManager::~RootSignatureManager()
{
}

bool RootSignatureManager::CreateRootSignature(ID3D12Device* device, std::wstring name)
{
    RootSignature rs;
    bool isCreated = rs.CreateRootSignature(device, name);

    if (isCreated)
    {
        mRootSignatures.emplace(name, rs);
    }

    return isCreated;
}

RootSignature & RootSignatureManager::GetRootSignature(std::wstring name)
{
    return mRootSignatures.at(name);
}
