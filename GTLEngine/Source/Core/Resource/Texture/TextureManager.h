#pragma once

// 텍스쳐 로드 및 SRV 저장 관리.
class UTextureManager
{

public:
	static HRESULT LoadTextureFromFile(const FString& FileName, ID3D11Device* Device, ID3D11ShaderResourceView** SRV);

};