#pragma once

class UDXDDepthStencilView
{
public:
	UDXDDepthStencilView() = default;
	~UDXDDepthStencilView();

	// TODO: desc 받도록 수정. 
	HRESULT CreateDepthStencilView(ComPtr<ID3D11Device> DXDDevice, HWND hWnd, UINT width, UINT height);
	void ReleaseDepthStencilView();

	ID3D11DepthStencilView* GetDepthStencilView() const { return DepthStencilView; }
	ID3D11ShaderResourceView* GetDepthStencilSRV() const { return DepthStencilSRV; }
private:
	ID3D11Texture2D*					DepthStencilBuffer = nullptr;
	ID3D11DepthStencilView*				DepthStencilView = nullptr;
	ID3D11ShaderResourceView*			DepthStencilSRV = nullptr;

};

