#include "pch.h"
#include "Material.h"

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
	AmbientTextureMap = InMatInfo.MapKa;
	DiffuseTextureMap = InMatInfo.MapKd;
	SpecularTextureMap = InMatInfo.MapKs;
	EmissiveTextureMap = InMatInfo.MapKe;
	BumpMap = InMatInfo.MapBump;
	DisplacementMap = InMatInfo.MapDisplace;
}
