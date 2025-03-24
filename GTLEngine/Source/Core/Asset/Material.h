#pragma once

#include "Object.h"
#include "Math/Vector.h"
#include "Asset/Mesh/StaticMeshAsset.h"

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
	void SetAmbientTextureMap(const FString& InAmbientTextureMap) { AmbientTextureMap = InAmbientTextureMap; }
	void SetDiffuseTextureMap(const FString& InDiffuseTextureMap) { DiffuseTextureMap = InDiffuseTextureMap; }
	void SetSpecularTextureMap(const FString& InSpecularTextureMap) { SpecularTextureMap = InSpecularTextureMap; }
	void SetEmissiveTextureMap(const FString& InEmissiveTextureMap) { EmissiveTextureMap = InEmissiveTextureMap; }
	void SetBumpMap(const FString& InBumpMap) { BumpMap = InBumpMap; }
	void SetDisplacementMap(const FString& InDisplacementMap) { DisplacementMap = InDisplacementMap; }

	FVector GetAmbient() const { return Ambient; }
	FVector GetDiffuse() const { return Diffuse; }
	FVector GetSpecular() const { return Specular; }
	FVector GetEmissive() const { return Emissive; }
	float GetSpecularExponent() const { return SpecularExponent; }
	int GetIlluminationModel() const { return IlluminationModel; }
	float GetOpacity() const { return Opacity; }
	float GetTransparency() const { return Transparency; }
	float GetRefractiveIndex() const { return RefractiveIndex; }
	FString GetAmbientTextureMap() const { return AmbientTextureMap; }
	FString GetDiffuseTextureMap() const { return DiffuseTextureMap; }
	FString GetSpecularTextureMap() const { return SpecularTextureMap; }
	FString GetEmissiveTextureMap() const { return EmissiveTextureMap; }
	FString GetBumpMap() const { return BumpMap; }
	FString GetDisplacementMap() const { return DisplacementMap; }
	
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
	FString AmbientTextureMap;
	FString DiffuseTextureMap;
	FString SpecularTextureMap;
	FString EmissiveTextureMap;
	FString BumpMap;
	FString DisplacementMap;
};