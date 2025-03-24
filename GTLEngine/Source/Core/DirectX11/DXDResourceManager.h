#pragma once

class UDXDResourceManager
{

public:
	UDXDResourceManager(ID3D11Device* Device);

	HRESULT CreateTextureSRV(const FString& FilePath);
	ID3D11ShaderResourceView* TryGetTextureSRV(const FString& Key);

	HRESULT CreateTextureSampler(const FString& Key, const D3D11_SAMPLER_DESC& InSamplerDesc);
	ID3D11SamplerState* TryGetTextureSampler(const FString& Key);


private:
	ID3D11Device* DXDDevice;
	TMap<FString, ID3D11ShaderResourceView*> TextureSRVPool;
	TMap<FString, ID3D11SamplerState*> TextureSamplerPool;


};