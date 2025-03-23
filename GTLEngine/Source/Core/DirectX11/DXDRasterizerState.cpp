#include "pch.h"
#include "DXDRasterizerState.h"

UDXDRasterizerState::~UDXDRasterizerState()
{
    ReleaseRasterizerState();
}

HRESULT UDXDRasterizerState::CreateRasterizerState(ComPtr<ID3D11Device> Device, const D3D11_RASTERIZER_DESC& Desc)
{
    HRESULT hr = Device->CreateRasterizerState(&Desc, &RasterizerState);
    if (FAILED(hr))
        return hr;
    
    return S_OK;
}

void UDXDRasterizerState::ReleaseRasterizerState()
{
    if (RasterizerState)
    {
        RasterizerState->Release();
		RasterizerState = nullptr;
    }
}
