#include "pch.h"
#include "Material.h"

#include "ObjectFactory.h"
#include "Texture.h"

TMap<FString, UMaterial*> FMaterialManager::MaterialPool;

void UMaterial::InitMaterial(const FObjMaterialInfo& InMatInfo)
{
	Ambient = InMatInfo.Ka;
	Diffuse = InMatInfo.Kd;
	Specular = InMatInfo.Ks;
	Emissive = InMatInfo.Ke;
	SpecularExponent = InMatInfo.Ns;
	IlluminationModel = InMatInfo.Illum;
	Opacity = InMatInfo.d;
	Transparency = InMatInfo.Tr;
	RefractiveIndex = InMatInfo.Ni;
	AmbientTextureMap = FTextureManager::LoadTexture(InMatInfo.MapKa);
	DiffuseTextureMap = FTextureManager::LoadTexture(InMatInfo.MapKd);
	SpecularTextureMap = FTextureManager::LoadTexture(InMatInfo.MapKs);
	EmissiveTextureMap = FTextureManager::LoadTexture(InMatInfo.MapKe);
	BumpMap = FTextureManager::LoadTexture(InMatInfo.MapBump);
	DisplacementMap = FTextureManager::LoadTexture(InMatInfo.MapDisplace);
}

UMaterial* FMaterialManager::LoadMaterial(const FString& MaterialName)
{
	if (MaterialPool.contains(MaterialName))
	{
		return MaterialPool[MaterialName];
	}
	return nullptr;
}

void FMaterialManager::CreateMaterial(const FString& MaterialName, const FObjMaterialInfo& InMatInfo)
{
	UMaterial* NewMaterial = FObjectFactory::ConstructObject<UMaterial>();
	NewMaterial->InitMaterial(InMatInfo);
	MaterialPool.insert({ MaterialName, NewMaterial });
}
