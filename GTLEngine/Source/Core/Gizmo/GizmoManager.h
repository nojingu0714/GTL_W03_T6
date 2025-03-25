#pragma once

#include "Object.h"

struct FRay;
class UDirectXHandle;

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

	void Init(UDirectXHandle* Handle);
	void Tick(float DeltaTime);
	void Destroy();

	void Picking();

private:
	GizmoState state;
};

