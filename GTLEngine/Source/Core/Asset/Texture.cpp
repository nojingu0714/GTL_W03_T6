#include "pch.h"
#include "Texture.h"
#include "ObjectFactory.h"

TMap<FString, UTexture*> FTextureManager::TexturePool;


UTexture* FTextureManager::LoadTexture(const FString& FileName)
{
	if (FileName == TEXT(""))
		return nullptr;
	if (TexturePool.contains(FileName))
	{
		return TexturePool[FileName];
	}
	UTexture* NewTexture = FObjectFactory::ConstructObject<UTexture>();
	NewTexture->SetTextureName(FileName);
	TexturePool.insert({ FileName, NewTexture });
    return TexturePool[FileName];
}
