#pragma once

#include "Actor.h"

class UStaticMeshComponent;

class AStaticMeshActor : public AActor
{
	DECLARE_CLASS(AStaticMeshActor, AActor)

public:
	AStaticMeshActor();

	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

	void SetStaticMesh(UStaticMeshComponent* InStaticMeshComponent);

	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

protected:
	UStaticMeshComponent* StaticMeshComponent = nullptr;

};
