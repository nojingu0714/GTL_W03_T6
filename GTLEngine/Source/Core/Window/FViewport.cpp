#include <pch.h>
#include "FViewport.h"
#include "DirectX11/DirectXHandle.h"
#include "Engine/Engine.h"

HRESULT FViewport::Init(const FString& InName, HWND hWnd, int InX, int InY, UINT InWidth, UINT InHeight)
{
	Name = InName;
	Viewport.TopLeftX = InX;
	Viewport.TopLeftY = InY;
	Viewport.Width = InWidth;
	Viewport.Height = InHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	
	UDirectXHandle* Handle = UEngine::GetEngine().GetDirectX11Handle();
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC TextureDesc = {};
	TextureDesc.Width = InWidth;
	TextureDesc.Height = InHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	hr = Handle->AddRenderTarget(InName, TextureDesc);
	if (FAILED(hr))
	{
		UE_LOG(TEXT("FViewport::Init::Failed to add render target"));
		return hr;
	}
	hr = Handle->AddDepthStencilView(InName, hWnd, InWidth, InHeight);
	if (FAILED(hr))
	{
		UE_LOG(TEXT("FViewport::Init::Failed to add render target"));
		return hr;
	}
	return hr;
}

void FViewport::SetDepthcomparisonMode(EDepthComparisonMode InDepthComparisonMode)
{
	DepthComparisonMode = InDepthComparisonMode;
}

void FViewport::SetRasterizerMode(ERasterizerMode InRasterizerMode)
{
	RasterizerMode = InRasterizerMode;
}

void FViewport::MoveViewport(int InX, int InY)
{
	Viewport.TopLeftX = InX;
	Viewport.TopLeftY = InY;
}

void FViewport::ResizeViewport(UINT InWidth, UINT InHeight)
{
	Viewport.Width = InWidth;
	Viewport.Height = InHeight;
}

void FViewport::SetProjectionMatrix(const FMatrix& InProjectionMatrix)
{
	CachedProjectionMatrix = InProjectionMatrix;
}
