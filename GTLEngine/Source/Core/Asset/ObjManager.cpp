#include "pch.h"
#include "ObjManager.h"
#include "Material.h"


TMap<FString, FStaticMesh*> FObjManager::ObjStaticMeshMap;

const int RenderMode = 3; // Triangle Render

std::string CreateVertexKey(int VertexIndex, int NormalIndex, int UVIndex)
{
    std::string key;
    return key.append(std::to_string(VertexIndex)).append(std::to_string(NormalIndex)).append(std::to_string(UVIndex));
}

FStaticMesh* FObjManager::LoadObjStaticMeshAsset(const FString& PathFileName)
{
    // 이미 있으면 바로 반환.
    if (ObjStaticMeshMap.contains(PathFileName))
    {
        return ObjStaticMeshMap[PathFileName];
    }
    // OBJ Parsing and create a new FStaticMesh
    // PathFileName으로 Obj 파싱하고 나온 ObjInfo로 FStaticMesh 생성해서 Map에 추가.

    // 없으면 만들어서 map에 캐싱 후 반환.
    size_t offset = PathFileName.find_last_of(L"/");
    std::string PathName = UGTLStringLibrary::WStringToString(PathFileName.substr(0, offset));
    std::string FileName = UGTLStringLibrary::WStringToString(PathFileName.substr(offset + 1, PathFileName.size()));

    FObjInfo NewObj;
    FObjImporter::ParseObjFile(PathName, FileName, NewObj);

    FStaticMesh* NewStaticMesh = FObjManager::ConvertObjToStaticMesh(NewObj);
	ObjStaticMeshMap.emplace(NewStaticMesh->PathFileName, NewStaticMesh);
	return ObjStaticMeshMap[PathFileName];
}

UStaticMesh* FObjManager::LoadObjStaticMesh(const FString& PathFileName)
{
    for (TObjectIterator<UStaticMesh> It; It; ++It)
    {
        UStaticMesh* StaticMesh = *It;
        if (StaticMesh->GetAssetPathFileName() == PathFileName)
            return *It;
    }

    FStaticMesh* Asset = FObjManager::LoadObjStaticMeshAsset(PathFileName);
    UStaticMesh* StaticMesh = FObjectFactory::ConstructObject<UStaticMesh>();
	StaticMesh->SetStaticMeshAsset(Asset);
	return StaticMesh;
}

UMaterial* FObjManager::LoadMaterial(const FString& PathFileName)
{
    return nullptr;
}



FStaticMesh* FObjManager::ConvertObjToStaticMesh(const FObjInfo& ObjInfo)
{
    FStaticMesh* NewStaticMesh = new FStaticMesh();
    NewStaticMesh->PathFileName = ObjInfo.PathFileName;

    int IndexCount = 0;

    for (const auto& MaterialFacePair : ObjInfo.FaceMap)
    {
        FString MaterialName = MaterialFacePair.first;
        const TArray<FFace>& Faces = MaterialFacePair.second;

        FStaticMeshSection NewSection;
        NewSection.MaterialName = MaterialName;

        IndexCount = 0;
        TMap<std::string, int> UniqueVertexMap;

        for (int b = 0;b < Faces.size();b++)
        {
            for (int a = 0;a < RenderMode;a++)
            {
            int VertexIndex = Faces[b].Vertices[a];
            int UVIndex = Faces[b].TexCoords[a];
            int NormalIndex = Faces[b].Normals[a];

            // 정점의 Key 생성 (정점, 노멀, UV를 기반으로 Key 생성)
            std::string VertexKey = CreateVertexKey(VertexIndex, NormalIndex, UVIndex);

            // 이미 존재하는 정점이면 건너뛰기
            if (UniqueVertexMap.contains(VertexKey)) 
            {
                NewSection.Indices.push_back(UniqueVertexMap[VertexKey]);
                continue;
            }

            // 새로운 정점이면 정점 정보를 추가
            FVector Position = ObjInfo.Vertices[VertexIndex];
            FVector4 Color = ObjInfo.Colors[VertexIndex];
            FVector Normal = ObjInfo.Normals[NormalIndex];
            FVector2 UV = ObjInfo.UV[UVIndex];

            FVertexPNCT Vertex(Position, Normal, Color, UV);
            NewSection.Vertices.push_back(Vertex);

            // 새로운 정점 인덱스 맵핑
            UniqueVertexMap[VertexKey]= IndexCount;
            NewSection.Indices.push_back(IndexCount);  // 새로 추가된 정점의 인덱스를 인덱스 버퍼에 추가

            IndexCount++;  // 인덱스 카운트를 증가
             }
        }
       
        NewStaticMesh->Sections.push_back(NewSection);
       
    }



    for (const FObjMaterialInfo& MaterialInfo : ObjInfo.Materials)
    {
        // MaterialInfo를 이용하여 Material을 생성하고 StaticMesh에 추가
        UMaterial* NewMaterial = new UMaterial();
        NewMaterial->InitMaterial(MaterialInfo);
		FMaterialManager::CreateMaterial(MaterialInfo.MatName, MaterialInfo);
    }

    return NewStaticMesh;
}

