#include "pch.h"
#include "GizmoScaleComponent.h"

#include "Asset/ObjManager.h"

UGizmoScaleMesh::UGizmoScaleMesh()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
}
