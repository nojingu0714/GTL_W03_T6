#pragma once

#include "Object.h"

struct FRay;

class UGizmoManager
{

public:
	UGizmoManager();

	void Tick(float DeltaTime);
	void Destroy();

	void Picking();

private:

};

