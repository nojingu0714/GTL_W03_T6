#pragma once

#include "Object.h"

class UTexture : public UObject
{

public:
	void SetTextureName(const FString& InTextureName) { TextureName = InTextureName; }
	FString GetTextureName() const { return TextureName; }
private:
	FString TextureName;

};


class FTextureManager
{
	static TMap<FString, UTexture*> TexturePool;

public:
	static UTexture* LoadTexture(const FString& FileName);

};