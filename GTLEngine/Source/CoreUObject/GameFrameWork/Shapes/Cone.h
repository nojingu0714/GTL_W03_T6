#pragma once

#include "GameFrameWork/StaticMeshActor.h"

class ACone : public AStaticMeshActor
{
	DECLARE_CLASS(ACone, AStaticMeshActor)
public:
	ACone();

public:
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;


};