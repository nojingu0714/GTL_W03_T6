#pragma once

#include "Mesh/StaticMeshAsset.h"
#include "Mesh/UStaticMesh.h"
#include "ObjectFactory.h"

struct FObjImporter
{
    // Obj Parsing (*.obj to FObjInfo)
    // Material Parsing (*.obj to MaterialInfo)
    // Convert the Raw data to Cooked data (FStaticMesh)
    static bool ParseObjFile(const std::string& InPath, const std::string& InFileName, FObjInfo& OutObjInfo);

    static bool ParseMtlFile(const std::string& PathName, const std::string& InFileName, TArray<FObjMaterialInfo>& InfoArray);


    static void SaveObjToBinary(const std::string& BinFileName, const FObjInfo& ObjInfo);

    static void LoadObjFromBinary(const std::string& BinFileName, FObjInfo& ObjInfo);
};

class FObjManager
{
private:
    static TMap<FString, FStaticMesh*> ObjStaticMeshMap; // TMap 기반의 메시 데이터

public:
    static FStaticMesh* ConvertObjToStaticMesh(const FObjInfo& ObjInfo);;

    // FStaticMesh를 로드하는 함수 (Cooked Data)
    static FStaticMesh* LoadObjStaticMeshAsset(const FString& PathFileName);

    static UStaticMesh* LoadObjStaticMesh(const FString& PathFileName);

};

