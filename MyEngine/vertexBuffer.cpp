#include "vertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D12Resource* vertexBufferResource, ID3D12Resource* vertexBufferUploadHeap, D3D12_SUBRESOURCE_DATA resourceData,
	int bufferSize, std::wstring name)
	: GPUResource(vertexBufferResource, vertexBufferUploadHeap, resourceData, bufferSize), mVertexBufferView({}), mName(name)
{
	bool isCreated = createVertexBufferView();
	ASSERT(isCreated);
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::createVertexBufferView()
{
	if (mBufferResource == NULL)
	{
		return false;
	}

	// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
	mVertexBufferView.BufferLocation = mBufferResource->GetGPUVirtualAddress();
	mVertexBufferView.StrideInBytes = sizeof(Vertex);
	mVertexBufferView.SizeInBytes = mBufferSize;

	return true;
}

D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::GetVertexBufferView()
{
    return mVertexBufferView;
}
