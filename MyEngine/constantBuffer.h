#pragma once

#include "stdafx.h"

// this is the structure of our constant buffer.
struct ConstantBufferPerObject {
    XMFLOAT4X4 wvpMat;
};

// Constant buffers must be 256-byte aligned which has to do with constant reads on the GPU.
// We are only able to read at 256 byte intervals from the start of a resource heap, so we will
// make sure that we add padding between the two constant buffers in the heap (one for cube1 and one for cube2)
// Another way to do this would be to add a float array in the constant buffer structure for padding. In this case
// we would need to add a float padding[50]; after the wvpMat variable. This would align our structure to 256 bytes (4 bytes per float)
// The reason i didn't go with this way, was because there would actually be wasted cpu cycles when memcpy our constant
// buffer data to the gpu virtual address. currently we memcpy the size of our structure, which is 16 bytes here, but if we
// were to add the padding array, we would memcpy 64 bytes if we memcpy the size of our structure, which is 50 wasted bytes
// being copied.
static int ConstantBufferPerObjectAlignedSize = (sizeof(ConstantBufferPerObject) + 255) & ~255;

class ConstantBuffer
{
private:
    ComPtr<ID3D12Resource> mConstantBufferUploadHeaps[NUM_OF_FRAME_BUFFERS]; // this is the memory on the gpu where constant buffers for each frame will be placed
    ConstantBufferPerObject* mCBPerObject; // this is the constant buffer data we will send to the gpu 
                                            // (which will be placed in the resources above)
    UINT8* mCBVGPUAddress[NUM_OF_FRAME_BUFFERS]; // this is a pointer to each of the constant buffer resource heaps

    std::wstring mName;

public:
    ConstantBuffer(ID3D12Resource* constantBufferUploadHeaps[NUM_OF_FRAME_BUFFERS], ConstantBufferPerObject* cbPerObject, UINT8* cbvGPUAddres[], std::wstring name);
    ~ConstantBuffer();

    ComPtr<ID3D12Resource> GetBufferResource(int pos) const;
    ConstantBufferPerObject* GetCBPerObject() const;
    UINT8* GetCBVGPUAddress(int pos) const;

    void StoreValues(XMMATRIX matIn);
};