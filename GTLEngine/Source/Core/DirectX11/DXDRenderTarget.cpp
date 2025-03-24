#include "pch.h"
#include "DXDRenderTarget.h"

UDXDRenderTarget::~UDXDRenderTarget()
{
    ReleaseRenderTarget();
}

HRESULT UDXDRenderTarget::CreateRenderTarget(ComPtr<ID3D11Device> DXDDevice, ComPtr<IDXGISwapChain> SwapChain, const D3D11_TEXTURE2D_DESC RenderTargetDesc, const D3D11_RENDER_TARGET_VIEW_DESC& RenderTargetViewDesc)
{
	HRESULT hr = S_OK;
    // render target이 되는 framebuffer의 사이즈가 0이면 자동으로 swapbuffer의 framebuffer를 이용합니다.
    if (RenderTargetDesc.Width == 0 && RenderTargetDesc.Height == 0)
    {
        hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)FrameBuffer.GetAddressOf());
        if(FAILED(hr))
            return hr;
    }
	// 특정 사이즈로 framebuffer를 생성합니다.
    else
    {
		hr = DXDDevice->CreateTexture2D(&RenderTargetDesc, nullptr, FrameBuffer.GetAddressOf());
		if (FAILED(hr))
			return hr;
    }
    hr = DXDDevice->CreateRenderTargetView(FrameBuffer.Get(), &RenderTargetViewDesc, FrameBufferRTV.GetAddressOf());
    if (FAILED(hr))
        return hr;

    // FrameBuffer는 Texture2D이므로, Shader에서 사용할 수 있도록 Shader Resource View를 생성.
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = RenderTargetDesc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;

    hr = DXDDevice->CreateShaderResourceView(FrameBuffer.Get(), &SRVDesc, FrameBufferResourceView.GetAddressOf());
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void UDXDRenderTarget::ReleaseRenderTarget()
{
    if (FrameBuffer)
    {
        FrameBuffer.Reset();
    }

    if (FrameBufferRTV)
    {
        FrameBufferRTV.Reset();
    }
}
