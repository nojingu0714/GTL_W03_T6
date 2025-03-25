#include "pch.h"
#include "GizmoActor.h"

#include "GizmoTranslateComponent.h"
#include "GizmoRotateComponent.h"
#include "GizmoScaleComponent.h"

AGizmoActor::AGizmoActor()
{
	GizmoTranslateComponent = AddComponent<UGizmoTranslateComponent>(this);
	GizmoRotateComponent = AddComponent<UGizmoRotateComponent>(this);
	GizmoScaleComponent = AddComponent<UGizmoScaleComponent>(this);
}
