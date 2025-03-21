#include "pch.h"
#include "GizmoRotate.h"

#include "Input/InputManager.h"
#include "CoreUObject/GameFrameWork/Camera.h"

UGizmoRotate::UGizmoRotate() : UGizmoBase() {
}

void UGizmoRotate::Init(EAxis axis, AActor* Target) {
	this->axis = axis;
	this->Target = Target;
	GizmoMode = EGizmoModeIndex::GMI_GizmoRotate;
	FVector Min = FVector(-0.120000f, -2.170000f, -2.170000f);	//하드코딩 min, max
	FVector Max = FVector(0.120000f, 2.170000f, 2.170000f);

	//X축 기준
	OriginalAABB[0] = FVector(Min.X, Min.Y, Min.Z);
	OriginalAABB[1] = FVector(Max.X, Min.Y, Min.Z);
	OriginalAABB[2] = FVector(Min.X, Max.Y, Min.Z);
	OriginalAABB[3] = FVector(Max.X, Max.Y, Min.Z);
	OriginalAABB[4] = FVector(Min.X, Min.Y, Max.Z);
	OriginalAABB[5] = FVector(Max.X, Min.Y, Max.Z);
	OriginalAABB[6] = FVector(Min.X, Max.Y, Max.Z);
	OriginalAABB[7] = FVector(Max.X, Max.Y, Max.Z);
	GizmoViewType = EGizmoViewType::XRotate;

	switch (axis) {
	case EAxis::X:
		break;
	case EAxis::Y:
		for (uint32 i = 0; i < 8; ++i)
			OriginalAABB[i] = FVector(OriginalAABB[i].Y, OriginalAABB[i].X, OriginalAABB[i].Z);
		GizmoViewType = EGizmoViewType::YRotate;
		break;
	case EAxis::Z:
		for (uint32 i = 0; i < 8; ++i)
			OriginalAABB[i] = FVector(OriginalAABB[i].Z, OriginalAABB[i].Y, OriginalAABB[i].X);
		GizmoViewType = EGizmoViewType::ZRotate;
		break;
	}
}

void UGizmoRotate::Tick(float TickTime)
{
	UGizmoBase::Tick(TickTime);
}

void UGizmoRotate::Destroy()
{
	UGizmoBase::Destroy();
}

void UGizmoRotate::OnClick(int mx, int my)
{
	startMouseX = mx;
	startMouseY = my;
}

void UGizmoRotate::OnDragTick(int mx, int my, int dmx, int dmy)
{
	// 단순 yaw pitch roll 변경
	//const float sensitive = 0.3f;
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
	//FVector directionInScreen = (mat * view * proj).TransformDirectionVector(orgDir);
	//float dx = dmx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width);
	//float dy = -dmy / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height);
	//float movement = FVector(dx, dy, 0).Dot(directionInScreen);
	//float deltaAngle = movement * sensitive;
	//FRotator deltaRotator(orgDir);
	//deltaRotator = deltaRotator * deltaAngle;
	//FRotator currentRotation = Target->GetActorRotation();
	//FRotator newRotation = currentRotation - (FRotator(mat.TransformDirectionVector(orgDir)) * deltaRotator);
	///*OutputDebugString((
	//	L"(" + std::to_wstring(directionInScreen.GetSafeNormal().X) +
	//	L"," + std::to_wstring(directionInScreen.GetSafeNormal().Y) +
	//	L"," + std::to_wstring(directionInScreen.GetSafeNormal().Z) + L")\n"
	//).c_str());*/
	//Target->SetActorRotation(newRotation);

	// Rotation Gizmo의 방향으로 움직이게 수정
	FRotator ActorRotation = Target->GetActorRotation();
	const float sensitive = 1.f;
	FVector RotationAxis;
	switch (axis) {
	case EAxis::X:
		RotationAxis = ActorRotation.TransformRotVecToMatrix(FVector(1, 0, 0));
		break;
	case EAxis::Y:
		RotationAxis = ActorRotation.TransformRotVecToMatrix(FVector(0,1,0));
		break;
	case EAxis::Z:
		RotationAxis = ActorRotation.TransformRotVecToMatrix(FVector(0,0,1));
		break;
	}

	// screen 상에서의 회전.
	FMatrix view = UEngine::GetEngine().GetWorld()->GetViewMatrix();
	FMatrix proj = UEngine::GetEngine().GetWorld()->GetProjectionMatrix();
	float x = mx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width)*2 - 1.0f;
	float y = my / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height)*(-2) + 1.0f;
	float dx = dmx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width);
	float dy = -dmy / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height);
	FVector MousePosNDC(x, y, 0);
	FVector MouseDeltaPosNDC(dx, dy, 0);
	FVector4 TargetPosNDC4((view * proj).TransformVector4(FVector4(Target->GetActorLocation(), 1.f)));
	FVector TargetPosNDC = TargetPosNDC4 / TargetPosNDC4.W;
	TargetPosNDC.Z = 0;
	FVector Displacement = MousePosNDC - TargetPosNDC;
	FVector RotationDirection = Displacement.Cross(MouseDeltaPosNDC); // z > 0 for clockwise, z < 0 for counter-clockwise.

	// direction of axis : toward or backward the screen.
    FVector AxisNDC = (view * proj).TransformVector4(FVector4(RotationAxis, 0.f)).xyz();
	RotationDirection.Z *= AxisNDC.Z;

	// slow rotation when the cursor is far from the gizmo.
	RotationDirection.Z /= Displacement.LengthSquared();

	// quaternions.
	RotationAxis.Normalize();
	FVector v = RotationAxis * sin(RotationDirection.Z * sensitive);
	FQuat RotationQuat(v.X, v.Y, v.Z, cos(RotationDirection.Z * sensitive));
	//RotationQuat.Normalize(); // already normalized.
	FQuat PreviousQuat = Target->GetActorRotation().Quaternion();
	//PreviousQuat.Normalize(); // already normalized.
	FQuat NewQuat = FQuat::MultiplyQuaternions(RotationQuat, PreviousQuat);
	//NewQuat.Normalize(); // already normalized.
	FRotator NewRotator(NewQuat);
	Target->SetActorRotation(NewRotator);
}

void UGizmoRotate::OnRelease(int mx, int my) {}

bool UGizmoRotate::IsClicked(FRay ray, float maxDistance, FVector& hitpoint)
{
	if (!Geometry::IsRayIntersectAABB(GetAABB(), ray, maxDistance))
		return false;

	USceneComponent* RootComp = Cast<USceneComponent>(Target->GetRootComponent());
	FMatrix transform;

	if (IsAbsoluteCoord)
		transform = RootComp->GetTranslateMatrix().Inverse();
	else
		transform = RootComp->GetTranslateMatrix().Inverse() * RootComp->GetRotationMatrix().Inverse();

	FRay transformedRay = FRay(
		transform.TransformPositionVector(ray.Origin),
		transform.TransformDirectionVector(ray.Direction).GetSafeNormal()
	);

	TArray<FVertexSimple> vertices = UEngine::GetEngine().GetResourceManager()->GetGizmoVertexData(GizmoViewType);
	TArray<uint32> indices = UEngine::GetEngine().GetResourceManager()->GetGizmoIndexData(GizmoViewType);
	if (indices.size() < 3)
		return false;

	bool result = false;

	int nPrimitives = indices.size() / 3;

	float nearHitDistancePow = FLT_MAX;
	FVector nearestHitPoint = FVector::Zero();
	for (int i = 0; i < nPrimitives; i++) {
		size_t idx0 = indices[static_cast<size_t>(i) * 3];
		size_t idx1 = indices[static_cast<size_t>(i) * 3 + 1];
		size_t idx2 = indices[static_cast<size_t>(i) * 3 + 2];

		FVector v0(vertices[idx0].X, vertices[idx0].Y, vertices[idx0].Z);
		FVector v1(vertices[idx1].X, vertices[idx1].Y, vertices[idx1].Z);
		FVector v2(vertices[idx2].X, vertices[idx2].Y, vertices[idx2].Z);

		FVector hit;
		if (Geometry::IsRayIntersectWithTriangle(transformedRay, v0, v1, v2, maxDistance, hit)) {
			if ((hitpoint - transformedRay.Origin).LengthSquared() < nearHitDistancePow) {
				nearHitDistancePow = (hitpoint - transformedRay.Origin).LengthSquared();
				hitpoint = hit;
			}
			result = true;
		}
	}
	return result;
}