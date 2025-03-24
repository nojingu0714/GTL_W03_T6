#include "pch.h"
#include "ObjManager.h"
#include "Material.h"

TMap<FString, FStaticMesh*> FObjManager::ObjStaticMeshMap;
TMap<FString, UMaterial*> FObjManager::MaterialMap;
TMap<FString, UTexture*> FObjManager::TextureMap;



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
    
    for (auto It = GUObjectArray.begin(); It != GUObjectArray.end(); ++It)
    {
        UStaticMesh* StaticMesh = Cast<UStaticMesh>(*It);
        if (StaticMesh && StaticMesh->GetAssetPathFileName() == PathFileName)
            return StaticMesh;
    }

    FStaticMesh* Asset = FObjManager::LoadObjStaticMeshAsset(PathFileName);
    UStaticMesh* StaticMesh = FObjectFactory::ConstructObject<UStaticMesh>();
	StaticMesh->SetStaticMeshAsset(Asset);
	return StaticMesh;
}



FStaticMesh* FObjManager::ConvertObjToStaticMesh(const FObjInfo& ObjInfo)
{
    FStaticMesh* NewStaticMesh = new FStaticMesh();
    NewStaticMesh->PathFileName = ObjInfo.PathFileName;

    for (const auto& MaterialFacePair : ObjInfo.FaceMap)
    {
        FString MaterialName = MaterialFacePair.first;
        const TArray<FFace>& Faces = MaterialFacePair.second;

        FStaticMeshSection NewSection;
        NewSection.MaterialName = MaterialName;

        TMap<int32, int32> VertexMap; // 중복 정점 방지용 맵
        int32 NextIndex = 0;

        for (const FFace& Face : Faces)
        {
            for (int32 i = 0; i < Face.Vertices.size(); ++i)
            {
                int32 VertexIndex = Face.Vertices[i];

                if (!VertexMap.contains(VertexIndex))
                {
                    FVector Position = ObjInfo.Vertices[VertexIndex];
                    FVector Normal = (VertexIndex < ObjInfo.Normals.size()) ? ObjInfo.Normals[VertexIndex] : FVector(0.0f, 0.0f, 1.0f);
                    FVector4 Color = (VertexIndex < ObjInfo.Colors.size()) ? ObjInfo.Colors[VertexIndex] : FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                    FVector2 UV = (VertexIndex < ObjInfo.UV.size()) ? ObjInfo.UV[VertexIndex] : FVector2(0.0f, 0.0f);

                    NewSection.Vertices.push_back(FNormalVertex(Position, Normal, Color, UV));
                    VertexMap[VertexIndex] = NextIndex;
                    NewSection.Indices.push_back(NextIndex++);
                }
                else
                {
                    NewSection.Indices.push_back(VertexMap[VertexIndex]);
                }
            }
        }

        // 하나의 Sub-Mesh(섹션) 추가
        NewStaticMesh->Sections.push_back(NewSection);
    }

    for (const FObjMaterialInfo& MaterialInfo : ObjInfo.Materials)
    {
        // MaterialInfo를 이용하여 Material을 생성하고 StaticMesh에 추가
        UMaterial* NewMaterial = new UMaterial();
        NewMaterial->InitMaterial(MaterialInfo);
        MaterialMap.insert({ MaterialInfo.MatName, NewMaterial });
    }

    return NewStaticMesh;
}