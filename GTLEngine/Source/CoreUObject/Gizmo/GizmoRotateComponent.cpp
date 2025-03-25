#include "pch.h"
#include "GizmoRotateComponent.h"

#include "Asset/ObjManager.h"

UGizmoRotateComponent::UGizmoRotateComponent()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotate.obj")));
}
