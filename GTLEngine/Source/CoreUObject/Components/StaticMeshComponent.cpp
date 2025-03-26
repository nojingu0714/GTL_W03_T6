#include "pch.h"
#include "StaticMeshComponent.h"

#include "Asset/ObjManager.h"
#include "Mesh/UStaticMesh.h"

UStaticMeshComponent::UStaticMeshComponent()
	: Super()
{
	StaticMesh = FObjManager::LoadObjStaticMesh(TEXT("Contents/Conan.obj"));
}

void UStaticMeshComponent::TickComponent(float TickTime)
{
	Super::TickComponent(TickTime);
	
}

void UStaticMeshComponent::Destroy()
{
	Super::Destroy();
}

FBoundingBox UStaticMeshComponent::GetAABB(const FMatrix& LocalToWorld) const
{
	FVector min = { FLT_MAX, FLT_MAX ,FLT_MAX };
	FVector max = { -FLT_MAX, -FLT_MAX ,-FLT_MAX };
	for (FVector v : StaticMesh->GetAsset()->Vertices)
	{
		v = LocalToWorld.TransformPositionVector(v);
		min.X = std::min(min.X, v.X);
		min.Y = std::min(min.Y, v.Y);
		min.Z = std::min(min.Z, v.Z);
		max.X = std::max(max.X, v.X);
		max.Y = std::max(max.Y, v.Y);
		max.Z = std::max(max.Z, v.Z);
	}
	return FBoundingBox(min, max);
}

void UStaticMeshComponent::SetStaticMesh(UStaticMesh* InStaticMesh)
{
	StaticMesh = InStaticMesh;
}

//void UStaticMeshComponent::Serialize(bool bIsLoading, json::JSON Handle)
//{
//    Super::Serialize(bIsLoading, Handle);
//
//    /*if (bIsLoading)
//    {
//        FString assetName;
//        Handle << "ObjStaticMeshAsset" << assetName;
//        StaticMesh = FObjManager::LoadObjStaticMesh(assetName);
//    }
//    else
//    {
//        FString assetName = StaticMesh->GetAssetPathFileName();
//        Handle << "ObjStaticMeshAsset" << assetName;
//    }*/
//}
