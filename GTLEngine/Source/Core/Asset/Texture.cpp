#include "pch.h"
#include "Texture.h"
#include "ObjectFactory.h"

TMap<FString, UTexture*> FTextureManager::TexturePool;


UTexture* FTextureManager::LoadTexture(const FString& FileName)
{

	if (TexturePool.contains(FileName))
	{
		return TexturePool[FileName];
	}
	UTexture* NewTexture = FObjectFactory::ConstructObject<UTexture>();
	TexturePool.insert({ FileName, NewTexture });
    return TexturePool[FileName];
}
