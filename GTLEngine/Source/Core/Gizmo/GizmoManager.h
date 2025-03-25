#pragma once

#include "Object.h"

struct FRay;
class UDirectXHandle;
class AGizmoActor;

enum class GizmoState
{
	None,
	Translate,
	Rotate,
	Scale,
};

class FGizmoManager
{

public:
	FGizmoManager();

	void Init();
	void Tick(float DeltaTime);
	void Destroy();

	void Picking();

	AGizmoActor* GetGizmoActor() const { return GizmoActor; }

private:
	GizmoState state;
	AGizmoActor* GizmoActor;
	
};

