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
private:
	ID3D11Texture2D*	DepthStencilBuffer;
	ID3D11DepthStencilView* DepthStencilView;
};

