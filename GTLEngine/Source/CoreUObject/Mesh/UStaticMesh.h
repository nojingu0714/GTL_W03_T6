#pragma once

#include "Object.h"

struct FStaticMesh;

class UStaticMesh : public UObject
{
	DECLARE_CLASS(UStaticMesh, UObject)

public:
	UStaticMesh();

	virtual void Destroy() override;

public:
	const FString& GetAssetPathFileName();
	void SetStaticMeshAsset(FStaticMesh* InStaticMeshAsset);
	const FStaticMesh* const GetAsset() { return StaticMeshAsset; }

private:
	FStaticMesh* StaticMeshAsset;

};
