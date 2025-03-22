#include "pch.h"
#include "TextureManager.h"

#include "DirectXTex/DirectXTex.h"

HRESULT UTextureManager::LoadTextureFromFile(const FString& FileName, ID3D11Device* Device, ID3D11ShaderResourceView** SRV)
{
	DirectX::ScratchImage Image;
	std::wstring ext = FileName.substr(FileName.find_last_of(L".") + 1);

	if (ext == L".dds" || ext == L".DDS")
	{
		if (FAILED(LoadFromDDSFile(FileName.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, Image)))
			return S_FALSE;
	}
	else if (ext == L".tga" || ext == L".TGA")
	{
		if (FAILED(LoadFromTGAFile(FileName.c_str(), nullptr, Image)))
			return S_FALSE;
	}
	else // WIC (png, jpg, jpeg, bmp )
	{
		if (FAILED(LoadFromWICFile(FileName.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, Image)))
			return S_FALSE;
	}

	//HRESULT hr = CreateShaderResourceView(Device, Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), SRV.GetAddressOf());
	return S_OK;
}
