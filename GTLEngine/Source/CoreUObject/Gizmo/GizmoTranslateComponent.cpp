#include "pch.h"
#include "GizmoTranslateComponent.h"

#include "Asset/ObjManager.h"

UGizmoTranslateComponent::UGizmoTranslateComponent()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
}
