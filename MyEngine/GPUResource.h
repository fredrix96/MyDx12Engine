#pragma once

#include "stdafx.h"

class GPUResource
{
protected:
    ComPtr<ID3D12Resource> mBufferResource;
    ComPtr<ID3D12Resource> mBufferUploadHeap;

    int mBufferSize;
    D3D12_SUBRESOURCE_DATA mResourceData;

public:
    GPUResource(ID3D12Resource* bufferResource, ID3D12Resource* bufferUploadHeap, D3D12_SUBRESOURCE_DATA resourceData, int bufferSize);
    virtual ~GPUResource();

    void CopyDataFromUploadHeapToDefaultHeap(ID3D12GraphicsCommandList* cmdList);

    ComPtr<ID3D12Resource> GetBufferResource() const;
};