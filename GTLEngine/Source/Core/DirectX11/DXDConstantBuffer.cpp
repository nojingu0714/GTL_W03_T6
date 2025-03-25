#include "pch.h"
#include "DXDConstantBuffer.h"

UDXDConstantBuffer::UDXDConstantBuffer(ComPtr<ID3D11Device> Device)
    : DXDDevice(Device)
    , ConstantBuffer(nullptr)
{}

void UDXDConstantBuffer::Release()
{
    if (ConstantBuffer)
    {
        ConstantBuffer->Release();
    }
}

HRESULT UDXDConstantBuffer::CreateBuffer(EConstantBufferType Type)
{
    if (!DXDDevice)
    {
        return S_FALSE;
    }

    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC BufferDesc = {};
    //ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

    switch (Type)
    {
    case EConstantBufferType::ChangesOnResize:
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.ByteWidth = sizeof(FCbChangesOnResize) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
        BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BufferDesc.MiscFlags = 0;
        BufferDesc.StructureByteStride = 0;
        break;
    case EConstantBufferType::ChangesEveryFrame:
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.ByteWidth = sizeof(FCbChangesEveryFrame) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
        BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BufferDesc.MiscFlags = 0;
        BufferDesc.StructureByteStride = 0;
        break;
    case EConstantBufferType::ChangesEveryObject:
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.ByteWidth = sizeof(FCbChangesEveryObject) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
        BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BufferDesc.MiscFlags = 0;
        BufferDesc.StructureByteStride = 0;
        break;
	case EConstantBufferType::Material:
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.ByteWidth = sizeof(FCbMaterial) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
    case EConstantBufferType::ViewportRatio:
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.ByteWidth = sizeof(FCbViewportRatio) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
        BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
	case EConstantBufferType::Gizmo:
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.ByteWidth = sizeof(FCbGizmo) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
    default:
        return S_FALSE;
        break;
    }   

    hr = DXDDevice->CreateBuffer(&BufferDesc, NULL, &ConstantBuffer);
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}
