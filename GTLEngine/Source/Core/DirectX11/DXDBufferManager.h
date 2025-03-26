#pragma once

#include "Resource/Types.h"

class UDXDBufferManager
{

public:
	UDXDBufferManager(ID3D11Device* DXDDevice = nullptr);

	template<typename T>
	HRESULT CreateVertexBuffer(const FString& KeyName, const TArray<T>& vertices, FVertexInfo& OutVertexInfo);

	HRESULT CreateIndexBuffer(const FString& KeyName, const TArray<uint32>& indices, FIndexInfo& OutIndexInfo);

	HRESULT CreateASCIITextBuffer(const FString& Text, FBufferInfo& OutBufferInfo, float WidthOffset, float HeightOffset);

	void ReleaseBuffers();
	template<typename T>
	HRESULT CreateDynamicVertexBuffer(const FString& KeyName, const TArray<T>& vertices, FVertexInfo& OutVertexInfo);

	FVertexInfo GetVertex(const FString& InName);
	FVertexInfo GetIndexBuffer(const FString& InName);

private:

	ID3D11Device* DXDDevice;

	TMap<FString, FVertexInfo> VertexBufferPool;
	TMap<FString, FIndexInfo> IndexBufferPool;
	TMap<FString, FBufferInfo> TextAtlasBufferPool;
};

template<typename T>
inline HRESULT UDXDBufferManager::CreateVertexBuffer(const FString& KeyName, const TArray<T>& vertices, FVertexInfo& OutVertexInfo)
{
	if (VertexBufferPool.contains(KeyName))
	{
		OutVertexInfo = VertexBufferPool[KeyName];
		return S_OK;
	}

	ID3D11Buffer* NewVertexBuffer;
	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(T) * static_cast<uint32>(vertices.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	HRESULT hr = DXDDevice->CreateBuffer(&bufferDesc, &initData, &NewVertexBuffer);
	if (FAILED(hr))
		return hr;

	OutVertexInfo.NumVertices = static_cast<uint32>(vertices.size());
	OutVertexInfo.VertexBuffer = NewVertexBuffer;

	VertexBufferPool.insert({ KeyName, { OutVertexInfo} });
	return S_OK;
}

template<typename T>
inline HRESULT UDXDBufferManager::CreateDynamicVertexBuffer(const FString& KeyName, const TArray<T>& vertices, FVertexInfo& OutVertexInfo)
{

	if (VertexBufferPool.contains(KeyName))
	{
		OutVertexInfo = VertexBufferPool[KeyName];
		return S_OK;
	}

	ID3D11Buffer* NewVertexBuffer;
	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(T) * static_cast<uint32>(vertices.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	HRESULT hr = DXDDevice->CreateBuffer(&bufferDesc, &initData, &NewVertexBuffer);
	if ( FAILED(hr) )
		return hr;

	OutVertexInfo.NumVertices = static_cast<uint32>(vertices.size());
	OutVertexInfo.VertexBuffer = NewVertexBuffer;

	VertexBufferPool.insert({ KeyName, { OutVertexInfo} });
	return S_OK;
}
