#include "pch.h"
#include "GizmoScaleComponent.h"

#include "Asset/ObjManager.h"

UGizmoScaleComponent::UGizmoScaleComponent()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
}
