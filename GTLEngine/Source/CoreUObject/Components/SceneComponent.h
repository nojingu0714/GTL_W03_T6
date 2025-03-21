#pragma once

#include "ActorComponent.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "Math/Matrix.h"

class USceneComponent : public UActorComponent
{
	DECLARE_CLASS(USceneComponent, UActorComponent)
public:
	USceneComponent();

	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:
	void SetRelativeLocation(const FVector& Loc);
	void SetRelativeRotation(const FRotator& Rot);
	void SetRelativeScale(const FVector& Scale);
	FVector GetComponentLocation() const { return RelativeLocation; }
	FRotator GetComponentRotation() const { return RelativeRotation; }
	FVector GetComponentScale() const { return RelativeScale; }

	FMatrix GetScaleMatrix() const;
	FMatrix GetRotationMatrix() const;
	FMatrix GetTranslateMatrix() const;
	FMatrix GetRotLocMatrix() const;
	FMatrix GetWorldMatrix() const;

private:
	FVector RelativeLocation;
	FRotator RelativeRotation;
	FVector RelativeScale;

public:
	USceneComponent* GetParent() const { return AttachParent; }
	TArray<USceneComponent*> GetAllChildren() const { return AttachChildren; }

	void SetupAttachment(class USceneComponent* InParent);

private:
	USceneComponent* AttachParent;
	TArray<USceneComponent*> AttachChildren;

};