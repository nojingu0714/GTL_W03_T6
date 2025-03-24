#pragma once

#include "Object.h"

class UTexture : public UObject
{
private:
	FString TextureName;

};


class FTextureManager
{
	static TMap<FString, UTexture*> TexturePool;

public:
	static UTexture* LoadTexture(const FString& FileName);

};