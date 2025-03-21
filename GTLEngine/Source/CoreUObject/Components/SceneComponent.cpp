#include "pch.h"
#include "SceneComponent.h"

#include "ObjectFactory.h"

USceneComponent::USceneComponent()
	: UActorComponent()
	, RelativeLocation(FVector::ZeroVector), RelativeRotation(FVector::ZeroVector), RelativeScale(FVector::OneVector)
{
	AttachParent = nullptr;
}

void USceneComponent::TickComponent(float TickTime)
{
	Super::TickComponent(TickTime);
}

void USceneComponent::Destroy()
{
	for (USceneComponent* Child : AttachChildren)
	{
		if (Child)
		{
			Child->Destroy();
			FObjectFactory::DestroyObject(Child);
		}
	}
}

void USceneComponent::SetRelativeLocation(const FVector& Loc)
{
	RelativeLocation = Loc;
}

void USceneComponent::SetRelativeRotation(const FRotator& Rot)
{
	RelativeRotation = Rot;
}

void USceneComponent::SetRelativeScale(const FVector& Scale)
{
	RelativeScale = Scale;
}

FMatrix USceneComponent::GetScaleMatrix() const
{
	FMatrix ScaleMat = FMatrix::GetScaleMatrix(RelativeScale);
	if (AttachParent)
	{
		FMatrix ParentScaleMat = AttachParent->GetScaleMatrix();
		ScaleMat = ScaleMat * ParentScaleMat;
	}
	return ScaleMat;
}

FMatrix USceneComponent::GetRotationMatrix() const
{
	FMatrix RotMat = FMatrix::GetRotateMatrix(RelativeRotation);
	if (AttachParent)
	{
		FMatrix ParentRotMat = AttachParent->GetRotationMatrix();
		RotMat = RotMat * ParentRotMat;
	}
	return RotMat;
}

FMatrix USceneComponent::GetTranslateMatrix() const
{
	FMatrix LocMat = FMatrix::GetTranslateMatrix(RelativeLocation);
	if (AttachParent)
	{
		FMatrix ParentLocMat = AttachParent->GetTranslateMatrix();
		LocMat = LocMat * ParentLocMat;
	}
	return LocMat;
}

FMatrix USceneComponent::GetRotLocMatrix() const
{
	FMatrix RotLocMat = GetRotationMatrix() * GetTranslateMatrix();
	if (AttachParent)
	{
		FMatrix ParentRotLocMat = AttachParent->GetRotLocMatrix();
		RotLocMat = RotLocMat * ParentRotLocMat;
	}
	return RotLocMat;
}

FMatrix USceneComponent::GetWorldMatrix() const
{
	FMatrix ScaleMat = FMatrix::GetScaleMatrix(GetComponentScale());
	FMatrix RotMat = FMatrix::GetRotateMatrix(GetComponentRotation());
	FMatrix TransMat = FMatrix::GetTranslateMatrix(GetComponentLocation());

	FMatrix RotTrs = RotMat * TransMat;
	USceneComponent* Parent = GetParent();
	while (Parent != nullptr) {
		FMatrix ParScaleMat = FMatrix::GetScaleMatrix(Parent->GetComponentScale());
		FMatrix ParRotMat = FMatrix::GetRotateMatrix(Parent->GetComponentRotation());
		FMatrix ParTransMat = FMatrix::GetTranslateMatrix(Parent->GetComponentLocation());

		ScaleMat = ScaleMat * ParScaleMat;

		FMatrix RTTemp = ParRotMat * ParTransMat;
		RotTrs = RotTrs * RTTemp;
		Parent = Parent->GetParent();
	}
	return ScaleMat * RotTrs;

}

void USceneComponent::SetupAttachment(USceneComponent* InParent)
{
	if (AttachParent != nullptr)
	{
		AttachParent->AttachChildren.erase(std::remove(AttachParent->AttachChildren.begin(), AttachParent->AttachChildren.end(), this), AttachParent->AttachChildren.end());
		AttachParent = nullptr;
	}

	if (InParent != AttachParent)
	{
		AttachParent = InParent;
		InParent->AttachChildren.push_back(this);
	}
}