#pragma once

#include "GameFrameWork/StaticMeshActor.h"

class ASphere : public AStaticMeshActor
{
	DECLARE_CLASS(ASphere, AStaticMeshActor)
public:
	ASphere();

public:
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

};