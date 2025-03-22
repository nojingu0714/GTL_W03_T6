#pragma once

#include "Mesh/StaticMeshAsset.h"
#include "Mesh/UStaticMesh.h"
#include "ObjectFactory.h"


class FObjManager
{
private:
    static TMap<FString, FStaticMesh*> ObjStaticMeshMap; // TMap 기반의 메시 데이터

public:
    // FStaticMesh를 로드하는 함수 (Cooked Data)
    static FStaticMesh* LoadObjStaticMeshAsset(const FString& PathFileName)
    {
        if (ObjStaticMeshMap.contains(PathFileName))
        {
            return ObjStaticMeshMap[PathFileName];
        }
        // OBJ Parsing and create a new FStaticMesh
        // PathFileName으로 Obj 파싱하고 나온 ObjInfo로 FStaticMesh 생성해서 Map에 추가.

        size_t offset = PathFileName.find_last_of(L"/");
        std::string PathName = UGTLStringLibrary::WStringToString(PathFileName.substr(0, offset));
        std::string FileName = UGTLStringLibrary::WStringToString(PathFileName.substr(offset + 1, PathFileName.size()));

        FObjInfo NewObj;
        FObjImporter::ParseObjFile(PathName, FileName, NewObj);

        // ObjInfo to FStaticMesh 해주기.


   //   ObjStaticMeshMap.Set(PathFileName, NewFStaticMesh);
    //    return NewFStaticMesh;
    }

   /* static UStaticMesh* LoadObjStaticMesh(const FString& PathFileName)
    {
        for (TObjectIterator<UStaticMesh> It; It; ++It)
        {
            UStaticMesh* StaticMesh = *It;
            if (StaticMesh->GetAssetPathFileName() == PathFileName)
                return It;
        }

        FStaticMesh* Asset = FObjManager::LoadObjStaticMeshAsset(PathFileName);
        UStaticMesh* StaticMesh = ConstructObject<UStaticMesh>();
        StaticMesh->SetStaticMeshAsset(StaticMeshAsset);
    }*/
};