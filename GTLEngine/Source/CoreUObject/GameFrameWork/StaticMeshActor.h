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

private:
	UStaticMeshComponent* StaticMeshComponent = nullptr;

};
