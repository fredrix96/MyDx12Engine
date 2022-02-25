#pragma once

#include "GPUResource.h"

class VertexBuffer : public GPUResource
{
private:
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
    std::wstring mName;

    bool createVertexBufferView();

public:
    VertexBuffer(ID3D12Resource* vertexBufferResource,
        ID3D12Resource* vertexBufferUploadHeap,
        D3D12_SUBRESOURCE_DATA resourceData,
        int bufferSize,
        std::wstring name);
    ~VertexBuffer() override;

    D3D12_VERTEX_BUFFER_VIEW & GetVertexBufferView();
};