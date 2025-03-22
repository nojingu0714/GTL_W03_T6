#include "pch.h"
#include "ObjManager.h"

std::unordered_map<std::wstring, FStaticMesh*> FObjManager::ObjStaticMeshMap;



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

    // StaticMesh의 PathFileName을 설정 (예시로 설정)
    NewStaticMesh->PathFileName = ObjInfo.PathFileName;

    // FNormalVertex 배열을 생성
    TArray<FNormalVertex> NormalVertices;

    // Vertices와 Normals, Colors를 기반으로 FNormalVertex 배열 생성
    for (int32 i = 0; i < ObjInfo.Vertices.size(); ++i)
    {
        FVector Position = ObjInfo.Vertices[i];
        FVector Normal = (i < ObjInfo.Normals.size()) ? ObjInfo.Normals[i] : FVector(0.0f, 0.0f, 1.0f); // 기본 노멀값
        FVector4 Color = (i < ObjInfo.Colors.size()) ? ObjInfo.Colors[i] : FVector4(1.0f, 1.0f, 1.0f, 1.0f); // 기본 색상값

        NormalVertices.push_back(FNormalVertex(Position, Normal, Color));
    }

    // Vertices를 StaticMesh에 추가
    NewStaticMesh->Vertices = NormalVertices;

    // FaceMap에서 면 인덱스를 추출하여 인덱스 배열을 생성
    TArray<uint32> Indices;
    for (const auto& MaterialFacePair : ObjInfo.FaceMap)
    {
        const TArray<FFace>& Faces = MaterialFacePair.second;

        for (const FFace& Face : Faces)
        {
            // 각 면에 대한 정점 인덱스를 Indices에 추가 (3개씩, 삼각형일 경우)
            for (int32 i = 0; i < Face.Vertices.size(); ++i)
            {
                Indices.push_back(Face.Vertices[i]);
            }
        }
    }

    // StaticMesh에 인덱스 추가
    NewStaticMesh->Indices = Indices;

    return NewStaticMesh;;
}