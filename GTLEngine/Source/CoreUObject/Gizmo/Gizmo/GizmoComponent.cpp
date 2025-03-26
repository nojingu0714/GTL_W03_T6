#include "pch.h"
#include "GizmoComponent.h"
#include "GameFrameWork/Actor.h"
#include "Asset/ObjManager.h"
#include "Asset/Mesh/StaticMeshAsset.h"
#include "Core/Input/InputManager.h"

#include "CoreUObject/Components/StaticMeshComponent.h"

UGizmoBaseComponent::UGizmoBaseComponent()
{
}

void UGizmoBaseComponent::Init()
{
}

void UGizmoBaseComponent::TickComponent(float TickTime)
{
}

void UGizmoBaseComponent::Destroy()
{
}

FVector UGizmoBaseComponent::GetHoveredHandleAxis(FRay Ray) const
{
	return FVector();
}

UGizmoTranslateComponent::UGizmoTranslateComponent()
{
}

void UGizmoTranslateComponent::Init()
{
	// Rendering용으로 사용할 StaticMeshComponent 생성.
	UStaticMeshComponent* GizmoTranslateMeshX = GetOwner()->AddComponent<UStaticMeshComponent>(GetOwner());
	GizmoTranslateMeshX->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshX->SetRelativeRotation(FRotator(0, 0, 0));
	GizmoTranslateMeshX->SetupAttachment(this);


	UStaticMeshComponent* GizmoTranslateMeshY = GetOwner()->AddComponent<UStaticMeshComponent>(GetOwner());
	GizmoTranslateMeshY->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshY->SetRelativeRotation(FRotator(0, 90, 0));
	GizmoTranslateMeshY->SetupAttachment(this);

	UStaticMeshComponent* GizmoTranslateMeshZ = GetOwner()->AddComponent<UStaticMeshComponent>(GetOwner());
	GizmoTranslateMeshZ->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshZ->SetRelativeRotation(FRotator(90, 0, 0));
	GizmoTranslateMeshZ->SetupAttachment(this);
}

void UGizmoTranslateComponent::TickComponent(float TickTime)
{
	
}

void UGizmoTranslateComponent::Destroy()
{
}

FVector UGizmoTranslateComponent::GetHoveredHandleAxis(FRay Ray) const
{
	FMatrix GizmoWorldMatrix = GetWorldMatrix();
	FVector BoxCenter = GetComponentLocation();
	BoxCenter = BoxCenter + (FVector(1,0,0) * 0.862);
	float distance;


	if (UEngine::GetEngine().GetInputManager()->GetMouseDown(UInputManager::EMouseButton::LEFT))
	{
		if (Geometry::IsRayIntersectWithBox(Ray, BoxCenter, FVector(1.724, 0.7, 0.7), GizmoWorldMatrix, distance))
		{
			UE_LOG(LogTemp, Warning, TEXT("Clicked"));
		}

	}


	return FVector();

}

UGizmoScaleComponent::UGizmoScaleComponent()
{
}

void UGizmoScaleComponent::Init()
{
}

void UGizmoScaleComponent::TickComponent(float TickTime)
{
}

void UGizmoScaleComponent::Destroy()
{
}

FVector UGizmoScaleComponent::GetHoveredHandleAxis(FRay Ray) const
{
	return FVector();
}

UGizmoRotateComponent::UGizmoRotateComponent()
{
}

void UGizmoRotateComponent::Init()
{
}

void UGizmoRotateComponent::TickComponent(float TickTime)
{
}

void UGizmoRotateComponent::Destroy()
{
}

FVector UGizmoRotateComponent::GetHoveredHandleAxis(FRay Ray) const
{
	return FVector();
}
