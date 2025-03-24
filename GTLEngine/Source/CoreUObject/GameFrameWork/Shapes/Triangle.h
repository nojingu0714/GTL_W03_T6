#pragma once

#include "GameFrameWork/StaticMeshActor.h"

class ATriangle : public AStaticMeshActor
{
	DECLARE_CLASS(ATriangle, AStaticMeshActor)
public:
	ATriangle();

public:
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

};