#include "pch.h"
#include "GizmoActor.h"

#include "GizmoComponent.h"
#include "GizmoStaticMeshComponent.h"
#include "Asset/ObjManager.h"
#include "Asset/Mesh/StaticMeshAsset.h"

AGizmoActor::AGizmoActor()
{
}

void AGizmoActor::Init()
{
	// Pivot
	Pivot = AddComponent<USceneComponent>(this);
	RootComponent = Pivot;

	GizmoTranslateComponent = AddComponent<UGizmoTranslateComponent>(this);
	GizmoRotateComponent = AddComponent<UGizmoRotateComponent>(this);
	GizmoScaleComponent = AddComponent<UGizmoScaleComponent>(this);

	GizmoTranslateComponent->SetupAttachment(Pivot);
	GizmoRotateComponent->SetupAttachment(Pivot);
	GizmoScaleComponent->SetupAttachment(Pivot);

	GizmoTranslateComponent->Init();
	GizmoRotateComponent->Init();
	GizmoScaleComponent->Init();

	//UGizmoTranslateComponent* TranslateCompY = AddComponent<UGizmoTranslateComponent>(this);
	//TranslateCompY->SetRelativeRotation(FRotator(0, 90, 0));
	//TranslateCompY->SetupAttachment(GizmoTranslateComponent);

	//UGizmoTranslateComponent* TranslateCompZ = AddComponent<UGizmoTranslateComponent>(this);
	//TranslateCompZ->SetRelativeRotation(FRotator(0, 0, 90));
	//TranslateCompZ->SetupAttachment(GizmoTranslateComponent);



}

void AGizmoActor::ProcessInput(FRay Ray)
{
	
	GizmoTranslateComponent->GetHoveredHandleAxis(Ray);
	GizmoRotateComponent->GetHoveredHandleAxis(Ray);
	GizmoScaleComponent->GetHoveredHandleAxis(Ray);

}

