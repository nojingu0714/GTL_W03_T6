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







    //// 기본 DepthStencilState
    //HRESULT hr = S_OK;

    //D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
    //memset(&DepthStencilDesc, 0, sizeof(DepthStencilDesc));
    //DepthStencilDesc.DepthEnable = TRUE;
    //DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    //hr = Device->CreateDepthStencilState(&DepthStencilDesc, &DefaultDepthStencilState);
    //if (FAILED(hr))
    //{
    //    return hr;
    //}

    //// 깊이 무시하는 DepthStencilState
    //memset(&DepthStencilDesc, 0, sizeof(DepthStencilDesc));
    //DepthStencilDesc.DepthEnable = FALSE;
    //DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    //DepthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    //hr = Device->CreateDepthStencilState(&DepthStencilDesc, &MaskZeroDepthStencilState);
    //if (FAILED(hr))
    //{
    //    return hr;
    //}

    //return S_OK;
}

void UDXDDepthStencilState::ReleaseDepthStencilState()
{
    if (DepthStencilState)
    {
        DepthStencilState->Release();
        DepthStencilState = nullptr;
    }
}