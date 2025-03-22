#include "pch.h"
#include "UStaticMesh.h"

#include "Resource/ObjManager.h"

UStaticMesh::UStaticMesh()
{
	StaticMeshAsset = FObjManager::LoadObjStaticMeshAsset(TEXT("Contents/cube-tex.obj"));
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
