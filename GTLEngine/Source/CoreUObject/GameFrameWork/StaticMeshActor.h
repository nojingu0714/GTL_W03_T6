#pragma once

#include "Actor.h"

class AStaticMeshActor : public AActor
{
	DECLARE_CLASS(AStaticMeshActor, AActor)

public:
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

};
