#include "direct3DContextManager.h"

Direct3DContextManager::Direct3DContextManager() :
	mVertexBufferMap(), mIndexBufferMap(), mHr(NULL)
{
}

Direct3DContextManager::~Direct3DContextManager()
{
}

bool Direct3DContextManager::CreateVertexBuffer(ID3D12Device* device, std::pair<Vertex*, int> vertexData, ID3D12GraphicsCommandList* cmdList, std::wstring name)
{
    int dataSize = vertexData.second;

    // create default heap
    // default heap is memory on the GPU. Only the GPU has access to this memory
    // To get data into this heap, we will have to upload the data using
    // an upload heap
    ID3D12Resource* vertexBufferResource;
    mHr = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
        D3D12_HEAP_FLAG_NONE, // no flags
        &CD3DX12_RESOURCE_DESC::Buffer(dataSize), // resource description for a buffer
        D3D12_RESOURCE_STATE_COPY_DEST, // we will start this heap in the copy destination state since we will copy data
                                        // from the upload heap to this heap
        nullptr, // optimized clear value must be null for this type of resource. used for render targets and depth/stencil buffers
        IID_PPV_ARGS(&vertexBufferResource));
    if (FAILED(mHr))
    {
        return false;
    }
    // we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
    std::wstring tmpName = L"vertexBufferResourceHeap_" + name;
    vertexBufferResource->SetName(tmpName.c_str());

    // create upload heap
    // upload heaps are used to upload data to the GPU. CPU can write to it, GPU can read from it
    // We will upload the vertex buffer using this heap to the default heap
    ID3D12Resource* vertexBufferUploadHeap;
    mHr = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
        D3D12_HEAP_FLAG_NONE, // no flags
        &CD3DX12_RESOURCE_DESC::Buffer(dataSize), // resource description for a buffer
        D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
        nullptr,
        IID_PPV_ARGS(&vertexBufferUploadHeap));
    if (FAILED(mHr))
    {
        return false;
    }
    tmpName = L"vertexBufferUploadHeap_" + name;
    vertexBufferUploadHeap->SetName(tmpName.c_str());

    // store vertex buffer in upload heap
    D3D12_SUBRESOURCE_DATA resourceData = {};
    resourceData.pData = reinterpret_cast<BYTE*>(vertexData.first); // pointer to our vertex array
    resourceData.RowPitch = dataSize; // size of all our triangle vertex data
    resourceData.SlicePitch = dataSize; // also the size of our triangle vertex data

    VertexBuffer tmpBuffer(vertexBufferResource, vertexBufferUploadHeap, resourceData, dataSize, name);
    tmpBuffer.CopyDataFromUploadHeapToDefaultHeap(cmdList);

    mVertexBufferMap.emplace(name, tmpBuffer);

    return true;
}

bool Direct3DContextManager::CreateIndexBuffer(ID3D12Device* device, std::pair<DWORD*, int> indexData, ID3D12GraphicsCommandList* cmdList, std::wstring name)
{
    int indexBufferSize = indexData.second;
    int numOfIndices = indexBufferSize / sizeof(DWORD);

    // create default heap to hold index buffer
    ID3D12Resource* indexBufferResource;
    mHr = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
        D3D12_HEAP_FLAG_NONE, // no flags
        &CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize), // resource description for a buffer
        D3D12_RESOURCE_STATE_COPY_DEST, // start in the copy destination state
        nullptr, // optimized clear value must be null for this type of resource
        IID_PPV_ARGS(&indexBufferResource));
    if (FAILED(mHr))
    {
        ASSERT(false);
        return false;
    }

    // we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
    std::wstring tmpName = L"indexBufferResourceHeap_" + name;
    indexBufferResource->SetName(tmpName.c_str());

    // create upload heap to upload index buffer
    ID3D12Resource* indexBufferUploadHeap;
    mHr = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
        D3D12_HEAP_FLAG_NONE, // no flags
        &CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize), // resource description for a buffer
        D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
        nullptr,
        IID_PPV_ARGS(&indexBufferUploadHeap));
    if (FAILED(mHr))
    {
        ASSERT(false);
        return false;
    }
    tmpName = L"indexBufferUploadHeap_" + name;
    indexBufferUploadHeap->SetName(tmpName.c_str());

    // store vertex buffer in upload heap
    D3D12_SUBRESOURCE_DATA resourceData = {};
    resourceData.pData = reinterpret_cast<BYTE*>(indexData.first); // pointer to our index array
    resourceData.RowPitch = indexBufferSize; // size of all our index buffer
    resourceData.SlicePitch = indexBufferSize; // also the size of our index buffer
    
    IndexBuffer tmpBuffer(indexBufferResource, indexBufferUploadHeap, resourceData, indexBufferSize, numOfIndices, name);
    tmpBuffer.CopyDataFromUploadHeapToDefaultHeap(cmdList);

    mIndexBufferMap.emplace(name, tmpBuffer);

    return true;
}

VertexBuffer Direct3DContextManager::GetVertexBuffer(std::wstring name) const
{
    return mVertexBufferMap.at(name);
}

IndexBuffer Direct3DContextManager::GetIndexBuffer(std::wstring name) const
{
    return mIndexBufferMap.at(name);
}
