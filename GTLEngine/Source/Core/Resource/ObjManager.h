#pragma once

#include "Mesh/StaticMeshAsset.h"

class UStaticMesh;

class FObjManager
{

private:
	static TMap<FString, FStaticMesh*> ObjStaticMeshMap;

public:
    static FStaticMesh* LoadObjStaticMeshAsset(const std::string& PathFileName)
    {
        if (It = ObjStaticMeshMap.Find(FileName))
        {
            return It;
        }
        // OBJ Parsing and create a new FStaticMesh
        ObjStaticMeshMap.Set(PathFileName, NewFStaticMesh);
        return NewFStaticMesh;
    }

    static UStaticMesh* LoadObjStaticMesh(const std::string& PathFileName)
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
    }


};