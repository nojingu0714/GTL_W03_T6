#pragma once

#include "GameFrameWork/StaticMeshActor.h"

class ACylinder : public AStaticMeshActor
{
	DECLARE_CLASS(ACylinder, AStaticMeshActor)
public:
	ACylinder();

public:
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

};