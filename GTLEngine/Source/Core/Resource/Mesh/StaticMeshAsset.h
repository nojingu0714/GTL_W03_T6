#pragma once

#include "Math/Vector.h"
#include <Resource/Types.h>

#include <fstream>
#include <sstream>

#include "Utils/GTLStringLibrary.h"

struct FNormalVertex
{
    FVector Position;      // 버텍스 포지션.
    FVector Normal;   // 버텍스 노말.
    FVector4 Color;
    FVector2 UV;     // 버텍스 텍스쳐 좌표.
};

struct FFace // obj 파일에 있는 f  a/b/c 값들 ( a :정점 / b : 텍스처 / c : 노멀 )
{
    TArray<int> Vertices;      // 정점 인덱스
    TArray<int> TexCoords;     // 텍스처 좌표 인덱스
    TArray<int> Normals;       // 노멀 벡터 인덱스
};



struct FStaticMesh
{
    FString PathFileName; // Mesh obj 파일 경로 이름.
    TArray<FNormalVertex> Vertices; // 버텍스 정보 (위치, 색상)
    TArray<uint32> Indices; // 인덱스 정보 (여기서는 비워둠)
};

struct FObjMaterialInfo
{
    FString MatName;
    FVector Ka; // Ambient Reflection
    FVector Kd; // Diffuse Reflection
    FVector Ks; // Specular Reflection
    FVector Ke; // Emissive Reflectivity
    float Ns = 0.0f; // Specular Exponent
    int Illum = 0; // Illumination Model
    float d = 1.0f; // 투명도 (opacity)
    float Tr = 0.0f; // 반비례 투명도 (Transparency)
    float Ni = 1.0f; // Refractive Index
    FString MapKa; // Ambient Texture Map
    FString MapKd; // Diffuse Texture Map
    FString MapKs; // Specular Texture Map
    FString MapKe; // Emissive Texture Map
    FString MapBump; // Bump Map
    FString MapDisplace; // Displacement Map

    // 필요에 따라 추가: 예) map_d, refl, bump scale(-bm), Tf 등.
};

// Raw Data. Obj 원본 데이터.
struct FObjInfo
{
	FString PathFileName; // Mesh obj 파일 경로 이름.
    TArray<FVector> Vertices;      // 버텍스 포지션.
    TArray<FVector> Normals;   // 버텍스 노말.
    TArray<FVector4> Colors;
    TArray<FVector2> UVs;
    TArray<FObjMaterialInfo> Materials;
	// 적용될 머티리얼 이름, face 정보.
    TMap<FString, TArray<FFace>> FaceMap;
};
