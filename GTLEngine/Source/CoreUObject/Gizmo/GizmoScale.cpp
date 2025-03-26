#include "pch.h"
#include "GizmoScale.h"
#include "Asset/ObjManager.h"
#include "CoreUObject/Components/StaticMeshComponent.h"

AGizmoScale::AGizmoScale() : AGizmoBase() {
}

void AGizmoScale::Init() {
	// Rendering용으로 사용할 StaticMeshComponent 생성.
	UStaticMeshComponent* GizmoScaleMeshX = AddComponent<UStaticMeshComponent>(this);
	GizmoScaleMeshX->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
	GizmoScaleMeshX->SetRelativeRotation(FRotator(0, 0, 0));
	GizmoScaleMeshX->SetupAttachment(RootComponent);


	UStaticMeshComponent* GizmoScaleMeshY = AddComponent<UStaticMeshComponent>(this);
	GizmoScaleMeshY->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
	GizmoScaleMeshY->SetRelativeRotation(FRotator(0, 90, 0));
	GizmoScaleMeshY->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoScaleMeshZ = AddComponent<UStaticMeshComponent>(this);
	GizmoScaleMeshZ->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
	GizmoScaleMeshZ->SetRelativeRotation(FRotator(90, 0, 0));
	GizmoScaleMeshZ->SetupAttachment(RootComponent);
}

void AGizmoScale::Tick(float TickTime)
{
	AGizmoBase::Tick(TickTime);
}

void AGizmoScale::Destroy()
{
	AGizmoBase::Destroy();
}

void AGizmoScale::OnClick(int mx, int my)
{
	startMouseX = mx;
	startMouseY = my;
}

void AGizmoScale::OnDragTick(FVector2 PointNDC, FVector2 DeltaNDC)
{
	//const float sensitive = 20.f;
	//FVector orgDir;
	//switch (axis) {
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

	//FMatrix view = UEngine::GetEngine().GetWorld()->GetViewMatrix();
	//FMatrix proj = UEngine::GetEngine().GetWorld()->GetProjectionMatrix();
	//FVector directionInScreen = (mat * view *  proj).TransformDirectionVector(orgDir);
	//float dx = dmx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width);
	//float dy = -dmy / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height);
	//float movement = FVector(dx, dy, 0).Dot(directionInScreen);
	///*OutputDebugString((
	//	L"(" + std::to_wstring(directionInScreen.GetSafeNormal().X) +
	//	L"," + std::to_wstring(directionInScreen.GetSafeNormal().Y) +
	//	L"," + std::to_wstring(directionInScreen.GetSafeNormal().Z) + L")\n"
	//).c_str());*/
	//FVector newScale = mat.TransformDirectionVector(orgDir) * movement * sensitive + Target->GetActorScale();
	//Target->SetActorScale(newScale);
}

void AGizmoScale::OnRelease(int mx, int my) {}

bool AGizmoScale::Intersects(FRay ray, float& hitDistance)
{
	return Geometry::IsRayIntersectAABB(GetAABB(), ray, hitDistance);
}
