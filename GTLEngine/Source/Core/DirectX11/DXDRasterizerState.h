#pragma once


/** Fill Mode(와이어프레임, 폴리곤 채우기) , Cull Mode(Back Front Both). */
class UDXDRasterizerState
{
public:
	UDXDRasterizerState() = default;
	~UDXDRasterizerState();

	
	HRESULT CreateRasterizerState(ComPtr<ID3D11Device> Device, const D3D11_RASTERIZER_DESC& Desc);
	void ReleaseRasterizerState();

	ID3D11RasterizerState* GetRasterizerState() const { return RasterizerState; }
private:
	ID3D11RasterizerState* RasterizerState;

};

