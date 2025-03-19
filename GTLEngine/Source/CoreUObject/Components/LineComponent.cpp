#include "pch.h"
#include "LineComponent.h"

ULineComponent::ULineComponent() {
	PrimitiveType = EPrimitiveType::Line;
}

void ULineComponent::Tick(float TickTime) {}

void ULineComponent::Destroy() {}

FBoundingBox ULineComponent::GetAABB() const {
	FVector min, max;
	FMatrix transformation = GetWorldMatrix();
	FVector start = transformation.TransformPositionVector(FVector::Zero());
	FVector end = transformation.TransformPositionVector(FVector(1.0f, 0.f, 0.f));

	min.X = (start.X < end.X) ? start.X : end.X;
	min.Y = (start.Y < end.Y) ? start.Y : end.Y;
	min.Z = (start.Z < end.Z) ? start.Z : end.Z;
	max.X = (start.X > end.X) ? start.X : end.X;
	max.Y = (start.Y > end.Y) ? start.Y : end.Y;
	max.Z = (start.Z > end.Z) ? start.Z : end.Z;
	return FBoundingBox(min, max);
}

bool ULineComponent::IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const {
	return false;
}

FMatrix ULineComponent::GetWorldMatrix() const {
	FMatrix ScaleMat = FMatrix::GetScaleMatrix(GetComponentScale());
	FMatrix RotMat = rotation;
	//FMatrix RotMat = FMatrix::GetRotateMatrix(GetComponentRotation());
	FMatrix TransMat = FMatrix::GetTranslateMatrix(GetComponentLocation());

	FMatrix RotTrs = RotMat * TransMat;
	USceneComponent* Parent = GetParent();
	while ( Parent != nullptr ) {
		FMatrix ParScaleMat = FMatrix::GetScaleMatrix(Parent->GetComponentScale());
		FMatrix ParRotMat = FMatrix::GetRotateMatrix(Parent->GetComponentRotation());
		FMatrix ParTransMat = FMatrix::GetTranslateMatrix(Parent->GetComponentLocation());

		ScaleMat = ScaleMat * ParScaleMat;

		FMatrix RTTemp = ParRotMat * ParTransMat;
		RotTrs = RotTrs * RTTemp;
		Parent = Parent->GetParent();
	}
	return ScaleMat * RotMat * TransMat;
}

FVector ULineComponent::GetStartPoint() const {
	return GetWorldMatrix().TransformPositionVector(FVector::ZeroVector);
}

FVector ULineComponent::GetEndPoint() const {
	return GetStartPoint() + GetWorldMatrix().TransformDirectionVector(FVector(1, 0, 0));
}
