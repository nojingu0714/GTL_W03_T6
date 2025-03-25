#include "pch.h"
#include "UStaticMesh.h"

#include "Asset/ObjManager.h"
#include "Asset/Mesh/StaticMeshAsset.h"


UStaticMesh::UStaticMesh()
{
	StaticMeshAsset = FObjManager::LoadObjStaticMeshAsset(TEXT("Contents/cube.obj"));
}

void UStaticMesh::Destroy()
{
	Super::Destroy();
}

const FString& UStaticMesh::GetAssetPathFileName()
{
	return StaticMeshAsset->PathFileName;
}

void UStaticMesh::SetStaticMeshAsset(FStaticMesh* InStaticMeshAsset)
{
	StaticMeshAsset = InStaticMeshAsset;
}
