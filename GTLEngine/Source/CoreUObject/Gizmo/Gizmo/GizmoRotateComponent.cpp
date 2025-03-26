#include "pch.h"
#include "GizmoRotateComponent.h"

#include "Asset/ObjManager.h"

UGizmoRotateMesh::UGizmoRotateMesh()
{
	SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotate.obj")));
}
