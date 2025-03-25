#include "pch.h"
#include "DXDDepthStencilState.h"

UDXDDepthStencilState::~UDXDDepthStencilState()
{
    ReleaseDepthStencilState();
}

HRESULT UDXDDepthStencilState::CreateDepthStencilState(const ComPtr<ID3D11Device> Device, const D3D11_DEPTH_STENCIL_DESC& InDesc)
{
    HRESULT hr = Device->CreateDepthStencilState(&InDesc, &DepthStencilState);
    if (FAILED(hr))
    {
        return hr;
    }
}

void UDXDDepthStencilState::ReleaseDepthStencilState()
{
    if (DepthStencilState)
    {
        DepthStencilState->Release();
        DepthStencilState = nullptr;
    }
}