#include "pch.h"
#include "DXDBufferManager.h"

HRESULT UDXDBufferManager::CreateIndexBuffer(ID3D11Device* Device, const TArray<uint32>& indices, FIndexInfo& OutIndexInfo)
{
	ID3D11Buffer* NewBuffer = nullptr;

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint32) * static_cast<uint32>(indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexInitData = {};
	indexInitData.pSysMem = indices.data();

	HRESULT hr = Device->CreateBuffer(&indexBufferDesc, &indexInitData, &NewBuffer);
	if (FAILED(hr))
		return hr;

	OutIndexInfo.NumIndices = static_cast<uint32>(indices.size());
	OutIndexInfo.IndexBuffer = NewBuffer;
	return S_OK;
}

HRESULT UDXDBufferManager::CreateASCIITextBuffer(ID3D11Device* Device, const FString& Text, FBufferInfo& OutBufferInfo, float WidthOffset, float HeightOffset)
{
	TArray<FVertexPT> Vertices;
	TArray<uint32> Indices;

	

	float gap = 1.0f / 16.0f;
	uint baseIndex = 0;

	float OffsetY = Text.size() * gap + WidthOffset; // 너비 오프셋. 가운데 정렬.
	float OffsetZ = 0.5f + HeightOffset; // 높이 오프셋.

	for (int i = 0; i < Text.size(); ++i)
	{
		// 현재 캐릭터의 아스키 코드.
		int CharacterCode = static_cast<int>(Text[i]);

		float u = (CharacterCode % 16) * gap;
		float v = (CharacterCode / 16) * gap;
		FVector Position;
		FVector2 UV;
		
		Vertices.push_back({ FVector(0.0f, OffsetY + 0.2f, OffsetZ - 0.2f), FVector2(u, v + gap) }); // 좌하단.
		Vertices.push_back({ FVector(0.0f, OffsetY + 0.2f, OffsetZ + 0.2f),  FVector2(u, v) }); // 좌상단.
		Vertices.push_back({ FVector(0.0f, OffsetY - 0.2f, OffsetZ + 0.2f),  FVector2(u + gap, v) }); // 우상단.
		Vertices.push_back({ FVector(0.0f, OffsetY - 0.2f, OffsetZ - 0.2f),  FVector2(u + gap, v + gap) }); // 우하단.

		Indices.push_back(baseIndex + 0);
		Indices.push_back(baseIndex + 1);
		Indices.push_back(baseIndex + 2);
		Indices.push_back(baseIndex + 0);
		Indices.push_back(baseIndex + 2);
		Indices.push_back(baseIndex + 3);

		OffsetY -= 0.2f;
		baseIndex += 4;
	}

	HRESULT hr = CreateVertexBuffer(Device, Vertices, OutBufferInfo.VertexInfo);
	if (FAILED(hr))
		return hr;

	hr = CreateIndexBuffer(Device, Indices, OutBufferInfo.IndexInfo);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void UDXDBufferManager::ReleaseBuffers()
{
	
}
