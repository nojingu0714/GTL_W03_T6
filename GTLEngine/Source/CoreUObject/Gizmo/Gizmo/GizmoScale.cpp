#include "pch.h"
#include "GizmoScale.h"

UGizmoScale::UGizmoScale() : UGizmoBase() {
}

void UGizmoScale::Init(EAxis axis, AActor* Target) {
	this->axis = axis;
	this->Target = Target;
	GizmoMode = EGizmoModeIndex::GMI_GizmoScale;
	FVector Min = FVector(0.000016f, -0.100000f, -0.100000f);	//하드코딩 min, max
	FVector Max = FVector(1.796750f, 0.100000f, 0.100000f);

	//X축 기준
	OriginalAABB[0] = FVector(Min.X, Min.Y, Min.Z);
	OriginalAABB[1] = FVector(Max.X, Min.Y, Min.Z);
	OriginalAABB[2] = FVector(Min.X, Max.Y, Min.Z);
	OriginalAABB[3] = FVector(Max.X, Max.Y, Min.Z);
	OriginalAABB[4] = FVector(Min.X, Min.Y, Max.Z);
	OriginalAABB[5] = FVector(Max.X, Min.Y, Max.Z);
	OriginalAABB[6] = FVector(Min.X, Max.Y, Max.Z);
	OriginalAABB[7] = FVector(Max.X, Max.Y, Max.Z);
	GizmoViewType = EGizmoViewType::XScale;

	switch (axis) {
	case EAxis::X:
		break;
	case EAxis::Y:
		for (uint32 i = 0; i < 8; ++i)
			OriginalAABB[i] = FVector(OriginalAABB[i].Y, OriginalAABB[i].X, OriginalAABB[i].Z);
		GizmoViewType = EGizmoViewType::YScale;
		break;
	case EAxis::Z:
		for (uint32 i = 0; i < 8; ++i)
			OriginalAABB[i] = FVector(OriginalAABB[i].Z, OriginalAABB[i].Y, OriginalAABB[i].X);
		GizmoViewType = EGizmoViewType::ZScale;
		break;
	}
}

void UGizmoScale::Tick(float TickTime)
{
	UGizmoBase::Tick(TickTime);
}

void UGizmoScale::Destroy()
{
	UGizmoBase::Destroy();
}

void UGizmoScale::OnClick(int mx, int my)
{
	startMouseX = mx;
	startMouseY = my;
}

void UGizmoScale::OnDragTick(int mx, int my, int dmx, int dmy)
{
	const float sensitive = 20.f;
	FVector orgDir;
	switch (axis) {
	case EAxis::X:
		orgDir = FVector(1, 0, 0);
		break;
	case EAxis::Y:
		orgDir = FVector(0, 1, 0);
		break;
	case EAxis::Z:
		orgDir = FVector(0, 0, 1);
		break;
	}

	FMatrix view = UEngine::GetEngine().GetWorld()->GetViewMatrix();
	FMatrix proj = UEngine::GetEngine().GetWorld()->GetProjectionMatrix();
	FVector directionInScreen = (mat * view *  proj).TransformDirectionVector(orgDir);
	float dx = dmx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width);
	float dy = -dmy / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height);
	float movement = FVector(dx, dy, 0).Dot(directionInScreen);
	/*OutputDebugString((
		L"(" + std::to_wstring(directionInScreen.GetSafeNormal().X) +
		L"," + std::to_wstring(directionInScreen.GetSafeNormal().Y) +
		L"," + std::to_wstring(directionInScreen.GetSafeNormal().Z) + L")\n"
	).c_str());*/
	FVector newScale = mat.TransformDirectionVector(orgDir) * movement * sensitive + Target->GetActorScale();
	Target->SetActorScale(newScale);
}

void UGizmoScale::OnRelease(int mx, int my) {}

bool UGizmoScale::IsClicked(FRay ray, float maxDistance, FVector& hitpoint)
{
	return Geometry::IsRayIntersectAABB(GetAABB(), ray, hitDistance);
}
