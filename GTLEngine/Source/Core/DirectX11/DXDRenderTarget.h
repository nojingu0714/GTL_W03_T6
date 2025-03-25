#pragma once

class UDXDRenderTarget
{

public:
    UDXDRenderTarget() = default;
    ~UDXDRenderTarget();

public:
    HRESULT CreateRenderTarget(ComPtr<ID3D11Device> DXDDevice, const D3D11_TEXTURE2D_DESC& RenderTargetDesc, const D3D11_RENDER_TARGET_VIEW_DESC& RenderTargetViewDesc);
    HRESULT CreateRenderTargetToSwapChain(ComPtr<ID3D11Device> DXDDevice, ComPtr<IDXGISwapChain> SwapChain);
    void ReleaseRenderTarget();


    //ComPtr<ID3D11Texture2D>             GetFrameBuffer() const { return FrameBuffer; }
    ComPtr<ID3D11RenderTargetView>      GetFrameBufferRTV() const { return FrameBufferRTV; }
    ComPtr<ID3D11ShaderResourceView>    GetFrameBufferSRV() const { return FrameBufferSRV; }
private:
    ComPtr<ID3D11Texture2D>             FrameBuffer = nullptr;
    ComPtr<ID3D11RenderTargetView>      FrameBufferRTV = nullptr;
    ComPtr<ID3D11ShaderResourceView>    FrameBufferSRV = nullptr;
};

