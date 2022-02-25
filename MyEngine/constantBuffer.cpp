#include "constantBuffer.h"

ConstantBuffer::ConstantBuffer(ID3D12Resource* constantBufferUploadHeaps[NUM_OF_FRAME_BUFFERS], ConstantBufferPerObject cbPerObject, UINT8* cbvGPUAddress[], std::wstring name) :
	mCBPerObject(cbPerObject), mName(name)
{
	for (int i = 0; i < NUM_OF_FRAME_BUFFERS; i++)
	{
		mConstantBufferUploadHeaps[i] = constantBufferUploadHeaps[i];
		mCBVGPUAddress[i] = cbvGPUAddress[i];
	}
}

ConstantBuffer::~ConstantBuffer()
{
}

ComPtr<ID3D12Resource> ConstantBuffer::GetBufferResource(int pos) const
{
	return mConstantBufferUploadHeaps[pos];
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGPUVirtualAddress(int pos) const
{
	return mConstantBufferUploadHeaps[pos].Get()->GetGPUVirtualAddress();
}

ConstantBufferPerObject & ConstantBuffer::GetCBPerObject()
{
	return mCBPerObject;
}

UINT8* ConstantBuffer::GetCBVGPUAddress(int pos) const
{
	return mCBVGPUAddress[pos];
}

void ConstantBuffer::StoreValues(XMMATRIX matIn)
{
	XMStoreFloat4x4(&mCBPerObject.wvpMat, matIn); // store transposed wvp matrix in constant buffer
}
