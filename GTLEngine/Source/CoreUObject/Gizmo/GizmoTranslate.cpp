#include "pch.h"
#include "GizmoTranslate.h"
#include "Asset/ObjManager.h"
#include "CoreUObject/Components/StaticMeshComponent.h"

AGizmoTranslate::AGizmoTranslate(): AGizmoBase() {}

void AGizmoTranslate::Init()
{
	// Rendering용으로 사용할 StaticMeshComponent 생성.
	UStaticMeshComponent* GizmoTranslateMeshX = AddComponent<UStaticMeshComponent>(this);
	GizmoTranslateMeshX->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshX->SetRelativeRotation(FRotator(0, 0, 0));
	GizmoTranslateMeshX->SetupAttachment(RootComponent);


	UStaticMeshComponent* GizmoTranslateMeshY = AddComponent<UStaticMeshComponent>(this);
	GizmoTranslateMeshY->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshY->SetRelativeRotation(FRotator(0, 90, 0));
	GizmoTranslateMeshY->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoTranslateMeshZ = AddComponent<UStaticMeshComponent>(this);
	GizmoTranslateMeshZ->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoTranslate.obj")));
	GizmoTranslateMeshZ->SetRelativeRotation(FRotator(90, 0, 0));
	GizmoTranslateMeshZ->SetupAttachment(RootComponent);
}

void AGizmoTranslate::Tick(float TickTime) {
	AGizmoBase::Tick(TickTime);
}

void AGizmoTranslate::Destroy() {
	AGizmoBase::Destroy();
}

void AGizmoTranslate::OnDragTick(FVector2 PointNDC, FVector2 DeltaNDC)
{
	//const float sensitive = 20.f;
	//FVector orgDir;
	//switch(axis) {
	//case EAxis::X:
	//	orgDir = FVector(1, 0, 0);
	//	break;
	//case EAxis::Y:
	//	orgDir = FVector(0, 1, 0);
	//	break;
	//case EAxis::Z:
	//	orgDir = FVector(0, 0, 1);
	//	break;
	//}
	//
	//FMatrix view = UEngine::GetEngine().GetWorld()->GetViewMatrix();
	//FMatrix proj = UEngine::GetEngine().GetWorld()->GetProjectionMatrix();
	//FVector directionInScreen = (mat * view * proj).TransformDirectionVector(orgDir);
	//float dx = dmx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width);
	//float dy = - dmy / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height);
	//float movement = FVector(dx, dy, 0).Dot(directionInScreen);
	///*OutputDebugString((
	//	L"(" + std::to_wstring(directionInScreen.GetSafeNormal().X) +
	//	L"," + std::to_wstring(directionInScreen.GetSafeNormal().Y) +
	//	L"," + std::to_wstring(directionInScreen.GetSafeNormal().Z) + L")\n"
	//).c_str());*/
	//FVector newPos = mat.TransformDirectionVector(orgDir) * movement * sensitive + Target->GetActorLocation();
	//Target->SetActorLocation(newPos);
}

bool AGizmoTranslate::Intersects(FRay ray, float& hitDistance) {
	return Geometry::IsRayIntersectAABB(GetAABB(), ray, hitDistance);
}
