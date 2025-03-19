#pragma once

#include "Resource/Types.h"

class UDXDBufferManager
{

public:
	template<typename T>
	HRESULT CreateVertexBuffer(ID3D11Device* Device, const TArray<T>& vertices, FVertexInfo& OutVertexInfo);

	HRESULT CreateIndexBuffer(ID3D11Device* Device, const TArray<uint32>& indices, FIndexInfo& OutIndexInfo);

	HRESULT CreateASCIITextBuffer(ID3D11Device* Device, const FString& Text, FBufferInfo& OutBufferInfo, float WidthOffset, float HeightOffset);

	void ReleaseBuffers();

};

template<typename T>
inline HRESULT UDXDBufferManager::CreateVertexBuffer(ID3D11Device* Device, const TArray<T>& vertices, FVertexInfo& OutVertexInfo)
{
	ID3D11Buffer* NewVertexBuffer;
	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(T) * static_cast<uint32>(vertices.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	HRESULT hr = Device->CreateBuffer(&bufferDesc, &initData, &NewVertexBuffer);
	if (FAILED(hr))
		return hr;

	OutVertexInfo.NumVertices = static_cast<uint32>(vertices.size());
	OutVertexInfo.VertexBuffer = NewVertexBuffer;

	return S_OK;
	
}
