#include "indexBuffer.h"

IndexBuffer::IndexBuffer(ID3D12Resource* indexBufferResource, ID3D12Resource* indexBufferUploadHeap, D3D12_SUBRESOURCE_DATA resourceData,
	int bufferSize, int numOfIndices, std::wstring name)
	: GPUResource(indexBufferResource, indexBufferUploadHeap, resourceData, bufferSize), mIndexBufferView({}), mName(name), mNumOfIndices(numOfIndices)
{
	bool isCreated = createIndexBufferView();
	ASSERT(isCreated);
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::createIndexBufferView()
{
	if (mBufferResource == NULL)
	{
		return false;
	}

	// create a index buffer view for the triangle. We get the GPU memory address to the index pointer using the GetGPUVirtualAddress() method
	mIndexBufferView.BufferLocation = mBufferResource->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R32_UINT; // 32-bit unsigned integer (this is what a dword is, double word, a word is 2 bytes)
	mIndexBufferView.SizeInBytes = mBufferSize;

	return true;
}

D3D12_INDEX_BUFFER_VIEW & IndexBuffer::GetIndexBufferView()
{
	return mIndexBufferView;
}

int IndexBuffer::GetNumOfIndices() const
{
	return mNumOfIndices;
}
