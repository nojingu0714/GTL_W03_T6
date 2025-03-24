#pragma once

#include "GameFrameWork/StaticMeshActor.h"


class ACube : public AStaticMeshActor
{
	DECLARE_CLASS(ACube, AStaticMeshActor)
public:
	ACube();

public:
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

};