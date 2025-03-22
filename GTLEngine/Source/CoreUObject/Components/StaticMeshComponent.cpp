#include "pch.h"
#include "StaticMeshComponent.h"

#include "Resource/ObjManager.h"
#include "Mesh/UStaticMesh.h"

#include "SimpleJSON/json.hpp"

void UStaticMeshComponent::TickComponent(float TickTime)
{
	Super::TickComponent(TickTime);
	
}

void UStaticMeshComponent::Destroy()
{
	Super::Destroy();
}

void UStaticMeshComponent::Serialize(bool bIsLoading, json::JSON Handle)
{
    Super::Serialize(bIsLoading, Handle);

    /*if (bIsLoading)
    {
        FString assetName;
        Handle << "ObjStaticMeshAsset" << assetName;
        StaticMesh = FObjManager::LoadObjStaticMesh(assetName);
    }
    else
    {
        FString assetName = StaticMesh->GetAssetPathFileName();
        Handle << "ObjStaticMeshAsset" << assetName;
    }*/
}
