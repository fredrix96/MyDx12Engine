#pragma once

#include "vertexBuffer.h"
#include "indexBuffer.h"

class Direct3DContextManager
{
private:
	std::map<std::wstring, VertexBuffer> mVertexBufferMap;
	std::map<std::wstring, IndexBuffer> mIndexBufferMap;

	HRESULT mHr;
public:

	Direct3DContextManager();
	~Direct3DContextManager();

	bool CreateVertexBuffer(ID3D12Device* device, std::pair<Vertex*, int> vertexData, ID3D12GraphicsCommandList* cmdList, std::wstring name);
	bool CreateIndexBuffer(ID3D12Device* device, std::pair<DWORD*, int> indexData, ID3D12GraphicsCommandList* cmdList, std::wstring name);

	VertexBuffer GetVertexBuffer(std::wstring name) const;
	IndexBuffer GetIndexBuffer(std::wstring name) const;
};
