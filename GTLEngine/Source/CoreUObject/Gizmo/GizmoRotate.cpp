#include "pch.h"
#include "GizmoRotate.h"
#include "Asset/ObjManager.h"
#include "CoreUObject/Components/StaticMeshComponent.h"


AGizmoRotate::AGizmoRotate() : AGizmoBase() {
}

void AGizmoRotate::Init() {
	// Rendering용으로 사용할 StaticMeshComponent 생성.
	UStaticMeshComponent* GizmoRotateMeshX = AddComponent<UStaticMeshComponent>(this);
	GizmoRotateMeshX->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotateNormalized.obj")));
	GizmoRotateMeshX->SetRelativeRotation(FRotator(90, 0, 0));
	GizmoRotateMeshX->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoRotateMeshY = AddComponent<UStaticMeshComponent>(this);
	GizmoRotateMeshY->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotateNormalized.obj")));
	GizmoRotateMeshY->SetRelativeRotation(FRotator(0, 0, -90));
	GizmoRotateMeshY->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoRotateMeshZ = AddComponent<UStaticMeshComponent>(this);
	GizmoRotateMeshZ->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoRotateNormalized.obj")));
	GizmoRotateMeshZ->SetRelativeRotation(FRotator(0, 0, 0));
	GizmoRotateMeshZ->SetupAttachment(RootComponent);
}

void AGizmoRotate::Tick(float TickTime)
{
	AGizmoBase::Tick(TickTime);
}

void AGizmoRotate::Destroy()
{
	AGizmoBase::Destroy();
}

void AGizmoRotate::OnClick(int mx, int my)
{
	startMouseX = mx;
	startMouseY = my;
}

void AGizmoRotate::OnDragTick(FVector2 PointNDC, FVector2 DeltaNDC)
{
	//// Rotation Gizmo의 방향으로 움직이게 수정
	//FRotator ActorRotation = Target->GetActorRotation();
	//const float sensitive = 1.f;
	//FVector RotationAxis;
	//switch (axis) {
	//case EAxis::X:
	//	RotationAxis = ActorRotation.TransformRotVecToMatrix(FVector(1, 0, 0));
	//	break;
	//case EAxis::Y:
	//	RotationAxis = ActorRotation.TransformRotVecToMatrix(FVector(0,1,0));
	//	break;
	//case EAxis::Z:
	//	RotationAxis = ActorRotation.TransformRotVecToMatrix(FVector(0,0,1));
	//	break;
	//}

	//// screen 상에서의 회전.
	//FMatrix view = UEngine::GetEngine().GetWorld()->GetViewMatrix();
	//FMatrix proj = UEngine::GetEngine().GetWorld()->GetProjectionMatrix();
	//float x = mx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width)*2 - 1.0f;
	//float y = my / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height)*(-2) + 1.0f;
	//float dx = dmx / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Width);
	//float dy = -dmy / static_cast<float>(UEngine::GetEngine().GetWindowInfo().Height);
	//FVector MousePosNDC(x, y, 0);
	//FVector MouseDeltaPosNDC(dx, dy, 0);
	//FVector4 TargetPosNDC4((view * proj).TransformVector4(FVector4(Target->GetActorLocation(), 1.f)));
	//FVector TargetPosNDC = TargetPosNDC4 / TargetPosNDC4.W;
	//TargetPosNDC.Z = 0;
	//FVector Displacement = MousePosNDC - TargetPosNDC;
	//FVector RotationDirection = Displacement.Cross(MouseDeltaPosNDC); // z > 0 for clockwise, z < 0 for counter-clockwise.

	//// direction of axis : toward or backward the screen.
 //   FVector AxisNDC = (view * proj).TransformVector4(FVector4(RotationAxis, 0.f)).xyz();
	//RotationDirection.Z *= AxisNDC.Z;

	//// slow rotation when the cursor is far from the gizmo.
	//RotationDirection.Z /= Displacement.LengthSquared();

	//// quaternions.
	//RotationAxis.Normalize();
	//FVector v = RotationAxis * sin(RotationDirection.Z * sensitive);
	//FQuat RotationQuat(v.X, v.Y, v.Z, cos(RotationDirection.Z * sensitive));
	////RotationQuat.Normalize(); // already normalized.
	//FQuat PreviousQuat = Target->GetActorRotation().Quaternion();
	////PreviousQuat.Normalize(); // already normalized.
	//FQuat NewQuat = FQuat::MultiplyQuaternions(RotationQuat, PreviousQuat);
	////NewQuat.Normalize(); // already normalized.
	//FRotator NewRotator(NewQuat);
	//Target->SetActorRotation(NewRotator);
}

void AGizmoRotate::OnRelease(int mx, int my) {}

bool AGizmoRotate::Intersects(FRay ray, float& hitDistance)
{
	if (!Geometry::IsRayIntersectAABB(GetAABB(), ray, hitDistance))
		return false;
	else
		return true;
	/*USceneComponent* RootComp = Cast<USceneComponent>(Target->GetRootComponent());
	FMatrix transform;

	if (IsAbsoluteCoord)
		transform = RootComp->GetTranslateMatrix().Inverse();
	else
		transform = RootComp->GetTranslateMatrix().Inverse() * RootComp->GetRotationMatrix().Inverse();

	FRay transformedRay = FRay(
		transform.TransformPositionVector(ray.Origin),
		transform.TransformDirectionVector(ray.Direction).GetSafeNormal()
	);

	TArray<FVertexSimple> vertices = FObjManager::LoadObjStaticMeshAsset(Get)
		GetGizmoVertexData(GizmoViewType);
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
	return result;*/
}