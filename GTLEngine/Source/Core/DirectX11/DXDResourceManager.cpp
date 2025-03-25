#include "pch.h"
#include "DXDResourceManager.h"
#include "DirectXTex/DirectXTex.h"

UDXDResourceManager::UDXDResourceManager(ID3D11Device* Device)
	: DXDDevice(Device)
{
}

HRESULT UDXDResourceManager::CreateTextureSRV(const FString& FilePath)
{
	if (FilePath.empty())
		return S_FALSE;
	FString ext = FilePath.substr(FilePath.find_last_of(TEXT(".")));

	DirectX::ScratchImage Image;

	if (ext == TEXT(".dds") || ext == TEXT(".DDS"))
	{
		if (FAILED(LoadFromDDSFile(FilePath.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, Image)))
			return S_FALSE;
	}
	else if (ext == TEXT(".tga") || ext == TEXT(".TGA"))
	{
		if (FAILED(LoadFromTGAFile(FilePath.c_str(), nullptr, Image)))
			return S_FALSE;
	}
	else // WIC (png, jpg, jpeg, bmp )
	{
		HRESULT hr = LoadFromWICFile(FilePath.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, Image);
		if (FAILED(hr))
			return S_FALSE;
	}

	ID3D11ShaderResourceView* NewSRV = nullptr;

	HRESULT hr = CreateShaderResourceView
	(DXDDevice, Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &NewSRV);

	if (FAILED(hr))
	{
		return hr;
	}

	TextureSRVPool.insert({ FilePath, NewSRV });
	return hr;
}


ID3D11ShaderResourceView* UDXDResourceManager::TryGetTextureSRV(const FString& Key)
{
	if (TextureSRVPool.contains(Key))
	{
		return TextureSRVPool[Key];
	};
	
	if (FAILED(CreateTextureSRV(Key)))
	{
		UE_LOG(LogTemp, Warning, TEXT("DXDResourceManager %s Texture Load Failed"), __FUNCTION__);
		return nullptr;
	}
	return TextureSRVPool[Key];
}

HRESULT UDXDResourceManager::CreateTextureSampler(const FString& Key, const D3D11_SAMPLER_DESC& InSamplerDesc)
{
	ID3D11SamplerState* NewSampler = nullptr;
	HRESULT hr = DXDDevice->CreateSamplerState(&InSamplerDesc, &NewSampler);
	if (FAILED(hr))
	{
		return hr;
	}
	TextureSamplerPool.insert({ Key, NewSampler });
	return S_OK;
}

ID3D11SamplerState* UDXDResourceManager::TryGetTextureSampler(const FString& Key)
{
	if (TextureSamplerPool.contains(Key))
	{
		return TextureSamplerPool[Key];
	}
	return nullptr;
}
