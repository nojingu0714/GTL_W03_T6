#include "pch.h"
//#include "ViewportClient.h"
//#include "Engine.h"
//#include "Core/DirectX11/DirectXHandle.h"
//#include "Core/DirectX11/DXDRenderTarget.h"
//#include "Core/DirectX11/DXDShaderManager.h"
//
//void FViewportClient::Init()
//{
//	//// Sampler State 생성 
//	//D3D11_SAMPLER_DESC SamplerDesc = {};
//	//SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
// //   SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
// //   SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
// //   SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
// //   SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
// //   SamplerDesc.MinLOD = 0;
// //   SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	//UEngine::GetEngine().GetDirectX11Handle()->GetD3DDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);
//
//	//// Quad Vertex Buffer 생성
//	//// 0  0     1  0
//	//// 0 -1     1 -1
//	//FVertexFont QuadVertices[] =
//	//{
//	//	{ FVector(1.f, -1.f, 0.f), FVector2(1.0f, 1.0f) }, // 우하단.
//	//	{ FVector(0.f, 0.f, 0.f), FVector2(0.0f, 0.0f) }, // 좌상단.
//	//	{ FVector(1.f, 0.f, 0.f), FVector2(1.0f, 0.0f) }, // 우상단.
//	//	{ FVector(1.f, -1.f, 0.f), FVector2(1.0f, 1.0f) }, // 우하단.
//	//	{ FVector(0.f, -1.f, 0.f), FVector2(0.0f, 1.0f) }, // 좌하단.
//	//	{ FVector(0.f, 0.f, 0.f), FVector2(0.0f, 0.0f) }, // 좌상단.
//	//};
//
//	//D3D11_BUFFER_DESC BufferDesc = {};
//	//BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	//BufferDesc.ByteWidth = sizeof(FVertexFont) * 6;
//	//BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	//D3D11_SUBRESOURCE_DATA InitData = {};
//	//InitData.pSysMem = QuadVertices;
//
//	//UEngine::GetEngine().GetDirectX11Handle()->GetD3DDevice()->CreateBuffer(&BufferDesc, &InitData, &QuadVertexBuffer);
//}
//
//void FViewportClient::Draw(const FString& ViewportName)
//{
//	UDirectXHandle* Handle = UEngine::GetEngine().GetDirectX11Handle();
//	ID3D11DeviceContext* Context = Handle->GetD3DDeviceContext();
//
//	UINT Stride = sizeof(FVertexFont);
//	UINT Offset = 0;
//
//	Context->IASetInputLayout(Handle->GetShaderManager()->GetInputLayoutByKey(TEXT("FontVS")));
//	Context->IASetVertexBuffers(0, 1, &QuadVertexBuffer, &Stride, &Offset);
//	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	// 셰이더 설정
//	Context->VSSetShader(Handle->GetShaderManager()->GetVertexShaderByKey(TEXT("ViewportVS")), nullptr, 0);
//	Context->PSSetShader(Handle->GetShaderManager()->GetPixelShaderByKey(TEXT("ViewportPS")), nullptr, 0);
//
//	// 텍스처 + 샘플러 바인딩
//	Context->PSSetShaderResources(0, 1, Handle->GetRenderTarget(ViewportName)->GetFrameBufferSRV().GetAddressOf());
//	Context->PSSetSamplers(0, 1, &SamplerState);
//
//	//UDirectXHandle* Handle = UEngine::GetEngine().GetDirectX11Handle();
//	//ID3D11DeviceContext* Context = Handle->GetD3DDeviceContext();
//
//	//UINT Stride = sizeof(FVertexFont);
//	//UINT Offset = 0;
//
//	//Context->IASetInputLayout(Handle->GetShaderManager()->GetInputLayoutByKey(TEXT("ViewportVS")));
//	//Context->IASetVertexBuffers(0, 1, &UEngine::GetEngine().ViewportClient->QuadVertexBuffer, &Stride, &Offset);
//	//Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//// 셰이더 설정
//	//Context->VSSetShader(Handle->GetShaderManager()->GetVertexShaderByKey(TEXT("ViewportVS")), nullptr, 0);
//	//Context->PSSetShader(Handle->GetShaderManager()->GetPixelShaderByKey(TEXT("ViewportPS")), nullptr, 0);
//
//	//// 텍스처 + 샘플러 바인딩
//	//Context->PSSetShaderResources(0, 1, Handle->GetRenderTarget(InViewport.GetName())->GetFrameBufferSRV().GetAddressOf());
//	//Context->PSSetSamplers(0, 1, &UEngine::GetEngine().ViewportClient->SamplerState);
//
//
//
//	//// viewport 위치 지정.
//	//D3D11_VIEWPORT viewport = InViewport.GetViewport();
//	//FWindowInfo windowInfo = UEngine::GetEngine().GetWindowInfo();
//	//ID3D11Buffer* CbViewportRatio = ConstantBuffers[EConstantBufferType::ChangesEveryObject]->GetConstantBuffer();
//	//if (!CbViewportRatio)
//	//{
//	//	return;
//	//}
//	//D3D11_MAPPED_SUBRESOURCE MappedData = {};
//	//DXDDeviceContext->Map(CbViewportRatio, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
//	//if (FCbViewportRatio* Buffer = reinterpret_cast<FCbViewportRatio*>(MappedData.pData))
//	//{
//	//	Buffer->x = InViewport.GetViewport().TopLeftX / windowInfo.Width * 2 - 1;
//	//	Buffer->y = InViewport.GetViewport().TopLeftY / windowInfo.Height * 2 - 1;
//	//	Buffer->width = InViewport.GetViewport().Width / windowInfo.Width * 2;
//	//	Buffer->height = InViewport.GetViewport().Height / windowInfo.Height * 2;
//	//}
//	//DXDDeviceContext->Unmap(CbViewportRatio, 0);
//
//	Context->Draw(6, 0);
//}
