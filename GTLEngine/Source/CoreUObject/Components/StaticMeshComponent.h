#pragma once

#include "MeshComponent.h"

class UStaticMesh;

class UStaticMeshComponent: public UMeshComponent
{
	DECLARE_CLASS(UStaticMeshComponent, UMeshComponent)
public:
	UStaticMeshComponent();

	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:
	// Primitive
	//void Serialize(bool bIsLoading, json::JSON Handle);

public:
	// Static Mesh
	void SetStaticMesh(UStaticMesh* InStaticMesh);
	UStaticMesh* GetStaticMesh() const { return StaticMesh; }

private:
	// 스태틱 메시 데이터.
	UStaticMesh* StaticMesh;

};