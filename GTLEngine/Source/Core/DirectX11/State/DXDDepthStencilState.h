#pragma once

class UDXDDepthStencilState
{
public:
	UDXDDepthStencilState() = default;
	~UDXDDepthStencilState();

	HRESULT CreateDepthStencilState(ComPtr<ID3D11Device> Device);
	void ReleaseDepthStencilState();

	ID3D11DepthStencilState* GetDefaultDepthStencilState() const { return DefaultDepthStencilState; }
	ID3D11DepthStencilState* GetMaskZeroDepthStencilState() const { return MaskZeroDepthStencilState; }

private:
	ID3D11DepthStencilState* DefaultDepthStencilState = nullptr;
	ID3D11DepthStencilState* MaskZeroDepthStencilState = nullptr;
};