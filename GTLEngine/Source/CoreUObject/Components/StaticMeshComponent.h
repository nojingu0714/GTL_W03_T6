#pragma once

#include "MeshComponent.h"

class UStaticMesh;

class UStaticMeshComponent: public UMeshComponent
{
	DECLARE_CLASS(UStaticMeshComponent, UMeshComponent)
public:
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:
	// 
	//void Serialize(bool bIsLoading, Json Handle);

private:
	// 스태틱 메시 데이터.
	UStaticMesh* StaticMesh;

};