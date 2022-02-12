#pragma once

#include "GPUResource.h"

class IndexBuffer : public GPUResource
{
private:
    D3D12_INDEX_BUFFER_VIEW mIndexBufferView = {};
    int mNumOfIndices;
    std::wstring mName;

    bool createIndexBufferView();

public:
    IndexBuffer(ID3D12Resource* indexBufferResource,
        ID3D12Resource* indexBufferUploadHeap,
        D3D12_SUBRESOURCE_DATA resourceData,
        int bufferSize,
        int numOfIndices,
        std::wstring name);
    ~IndexBuffer() override;

    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
    int GetNumOfIndices() const;
};
