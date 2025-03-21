#include "pch.h"
#include "UStaticMesh.h"

void UStaticMesh::Destroy()
{
	Super::Destroy();
}

const FString& UStaticMesh::GetAssetPathName()
{
	return StaticMeshAsset->PathFileName;
}

void UStaticMesh::SetStaticMeshAsset(FStaticMesh* InStaticMeshAsset)
{
	StaticMeshAsset = InStaticMeshAsset;
}
