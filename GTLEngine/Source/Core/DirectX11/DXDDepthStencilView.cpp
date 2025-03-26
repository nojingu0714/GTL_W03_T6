#include "pch.h"
#include "DXDRasterizerState.h"

#include "Engine.h"
#include "DXDDepthStencilView.h"

UDXDDepthStencilView::~UDXDDepthStencilView()
{
    ReleaseDepthStencilView();
}

HRESULT UDXDDepthStencilView::CreateDepthStencilView(ComPtr<ID3D11Device> Device, UINT width, UINT height)
{
    uint32 Width = static_cast<uint32>(width);
    uint32 Height = static_cast<uint32>(height);

    HRESULT hr = S_OK;

    D3D11_TEXTURE2D_DESC DepthStencilBufferDesc = {};
    // TODO: SwapChain에서 받아서 똑같이 맞춰줘야 함.
    DepthStencilBufferDesc.Width = Width;
    DepthStencilBufferDesc.Height = Height;
    DepthStencilBufferDesc.MipLevels = 1;
    DepthStencilBufferDesc.ArraySize = 1;
    DepthStencilBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    DepthStencilBufferDesc.SampleDesc.Count = 1;
    DepthStencilBufferDesc.SampleDesc.Quality = 0;
    DepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    DepthStencilBufferDesc.CPUAccessFlags = 0;
    DepthStencilBufferDesc.MiscFlags = 0;

    hr = Device->CreateTexture2D(&DepthStencilBufferDesc, nullptr, &DepthStencilBuffer);
    if (FAILED(hr))
    {
        return hr;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
    ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
    DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DepthStencilViewDesc.Texture2D.MipSlice = 0;

    hr = Device->CreateDepthStencilView(DepthStencilBuffer, &DepthStencilViewDesc, &DepthStencilView);
    if (FAILED(hr))
    {
        return hr;
    }

    // 2. Depth Texture의 SRV
    D3D11_SHADER_RESOURCE_VIEW_DESC depthSRVDesc = {};
    depthSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;  // 깊이 값만 사용
    depthSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    depthSRVDesc.Texture2D.MipLevels = 1;

    hr = Device->CreateShaderResourceView(DepthStencilBuffer, &depthSRVDesc, &DepthStencilSRV);
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

void UDXDDepthStencilView::ReleaseDepthStencilView()
{
    if (DepthStencilBuffer)
    {
        DepthStencilBuffer->Release();
		DepthStencilBuffer = nullptr;
    }

    if (DepthStencilView)
    {
        DepthStencilView->Release();
		DepthStencilView = nullptr;
    }
    if (DepthStencilSRV)
    {
        DepthStencilSRV->Release();
        DepthStencilSRV = nullptr;
    }
}
