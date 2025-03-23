#pragma once

//class UDXDDepthStencilState
//{
//public:
//	UDXDDepthStencilState() = default;
//	~UDXDDepthStencilState();
//
//	HRESULT CreateDepthStencilState(ComPtr<ID3D11Device> Device);
//	void ReleaseDepthStencilState();
//
//	ID3D11DepthStencilState* GetDefaultDepthStencilState() const { return DefaultDepthStencilState; }
//	ID3D11DepthStencilState* GetMaskZeroDepthStencilState() const { return MaskZeroDepthStencilState; }
//
//private:
//	ID3D11DepthStencilState* DefaultDepthStencilState = nullptr;
//	ID3D11DepthStencilState* MaskZeroDepthStencilState = nullptr;
//};


/** 깊이 테스트 활성화, 쓰기, 비교 방식 등 */
class UDXDDepthStencilState
{
public:
	UDXDDepthStencilState() = default;
	~UDXDDepthStencilState();

	// TODO: desc 받도록 수정. 
	HRESULT CreateDepthStencilState(const ComPtr<ID3D11Device> Device, const D3D11_DEPTH_STENCIL_DESC& InDesc);
	void ReleaseDepthStencilState();

	ID3D11DepthStencilState* GetDepthStencilState() const { return DepthStencilState; }
private:
	ID3D11DepthStencilState* DepthStencilState = nullptr;
};

