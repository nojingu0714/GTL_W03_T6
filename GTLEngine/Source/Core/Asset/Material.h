#pragma once

#include "Object.h"
#include "Math/Vector.h"
#include "Asset/Mesh/StaticMeshAsset.h"

class UTexture;

class UMaterial : public UObject
{
	DECLARE_CLASS(UMaterial, UObject)

public:
	UMaterial() = default;

	void InitMaterial(const FObjMaterialInfo& InMatInfo);

	void SetAmbient(const FVector& InAmbient) { Ambient = InAmbient; }
	void SetDiffuse(const FVector& InDiffuse) { Diffuse = InDiffuse; }
	void SetSpecular(const FVector& InSpecular) { Specular = InSpecular; }
	void SetEmissive(const FVector& InEmissive) { Emissive = InEmissive; }
	void SetSpecularExponent(float InSpecularExponent) { SpecularExponent = InSpecularExponent; }
	void SetIlluminationModel(int InIlluminationModel) { IlluminationModel = InIlluminationModel; }
	void SetOpacity(float InOpacity) { Opacity = InOpacity; }
	void SetTransparency(float InTransparency) { Transparency = InTransparency; }
	void SetRefractiveIndex(float InRefractiveIndex) { RefractiveIndex = InRefractiveIndex; }
	void SetAmbientTextureMap(UTexture* InAmbientTextureMap) { AmbientTextureMap = InAmbientTextureMap; }
	void SetDiffuseTextureMap(UTexture* InDiffuseTextureMap) { DiffuseTextureMap = InDiffuseTextureMap; }
	void SetSpecularTextureMap(UTexture* InSpecularTextureMap) { SpecularTextureMap = InSpecularTextureMap; }
	void SetEmissiveTextureMap(UTexture* InEmissiveTextureMap) { EmissiveTextureMap = InEmissiveTextureMap; }
	void SetBumpMap(UTexture* InBumpMap) { BumpMap = InBumpMap; }
	void SetDisplacementMap(UTexture* InDisplacementMap) { DisplacementMap = InDisplacementMap; }

	FVector GetAmbient() const { return Ambient; }
	FVector GetDiffuse() const { return Diffuse; }
	FVector GetSpecular() const { return Specular; }
	FVector GetEmissive() const { return Emissive; }
	float GetSpecularExponent() const { return SpecularExponent; }
	int GetIlluminationModel() const { return IlluminationModel; }
	float GetOpacity() const { return Opacity; }
	float GetTransparency() const { return Transparency; }
	float GetRefractiveIndex() const { return RefractiveIndex; }
	UTexture* GetAmbientTextureMap() const { return AmbientTextureMap; }
	UTexture* GetDiffuseTextureMap() const { return DiffuseTextureMap; }
	UTexture* GetSpecularTextureMap() const { return SpecularTextureMap; }
	UTexture* GetEmissiveTextureMap() const { return EmissiveTextureMap; }
	UTexture* GetBumpMap() const { return BumpMap; }
	UTexture* GetDisplacementMap() const { return DisplacementMap; }
	
private:
	FVector Ambient;
	FVector Diffuse;
	FVector Specular;
	FVector Emissive;
	float SpecularExponent;
	int IlluminationModel;
	float Opacity;
	float Transparency;
	float RefractiveIndex;
	UTexture* AmbientTextureMap;
	UTexture* DiffuseTextureMap;
	UTexture* SpecularTextureMap;
	UTexture* EmissiveTextureMap;
	UTexture* BumpMap;
	UTexture* DisplacementMap;
};

class FMaterialManager
{
	static TMap<FString, UMaterial*> MaterialPool;

public:
	static UMaterial* LoadMaterial(const FString& MaterialName);

	static void CreateMaterial(const FString& MaterialName, const FObjMaterialInfo& InMatInfo);

};