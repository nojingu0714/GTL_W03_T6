#include "pch.h"
#include "ViewportClient.h"
#include "Engine.h"
#include "Core/DirectX11/DirectXHandle.h"
#include "Core/DirectX11/DXDRenderTarget.h"
#include "Core/DirectX11/DXDShaderManager.h"

void FViewportClient::Init()
{
	// Sampler State 생성 
	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SamplerDesc.MinLOD = 0;
    SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	UEngine::GetEngine().GetDirectX11Handle()->GetD3DDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);

	// Quad Vertex Buffer 생성
	FVertexUV QuadVertices[] =
	{
		{ 0.0f, 0.5f, -0.5f, 0.0f, 1.0f }, // 좌하단.
		{ 0.0f, 0.5f, 0.5f, 0.0f, 0.0f }, // 좌상단.
		{ 0.0f, -0.5f, 0.5f, 1.0f, 0.0f }, // 우상단.
		{ 0.0f, 0.5f, -0.5f, 0.0f, 1.0f }, // 좌하단.
		{ 0.0f, -0.5f, 0.5f, 1.0f, 0.0f }, // 우상단.
		{ 0.0f, -0.5f, -0.5f, 1.0f, 1.0f }, // 우하단.
	};

	D3D11_BUFFER_DESC BufferDesc = {};
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = sizeof(FVertexUV) * 6;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = QuadVertices;

	UEngine::GetEngine().GetDirectX11Handle()->GetD3DDevice()->CreateBuffer(&BufferDesc, &InitData, &QuadVertexBuffer);
}

void FViewportClient::Draw(const FString& ViewportName)
{
	UDirectXHandle* Handle = UEngine::GetEngine().GetDirectX11Handle();
	ID3D11DeviceContext* Context = Handle->GetD3DDeviceContext();

	UINT Stride = sizeof(FVertexUV);
	UINT Offset = 0;

	Context->IASetInputLayout(Handle->GetShaderManager()->GetInputLayoutByKey(TEXT("ViewportVS")));
	Context->IASetVertexBuffers(0, 1, &QuadVertexBuffer, &Stride, &Offset);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// 셰이더 설정
	Context->VSSetShader(Handle->GetShaderManager()->GetVertexShaderByKey(TEXT("ViewportVS")), nullptr, 0);
	Context->PSSetShader(Handle->GetShaderManager()->GetPixelShaderByKey(TEXT("ViewportPS")), nullptr, 0);

	// 텍스처 + 샘플러 바인딩
	Context->PSSetShaderResources(0, 1, Handle->GetRenderTarget(ViewportName)->GetFrameBufferResourceView().GetAddressOf());
	Context->PSSetSamplers(0, 1, &SamplerState);

	Context->Draw(6, 0);
}
