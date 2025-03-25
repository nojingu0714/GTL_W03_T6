#include "pch.h"
#include "Actor.h"

#include "Core/Engine/Engine.h"
#include "CoreUObject/World.h"
#include "Utils/Math/Geometry.h"
#include "Components/SceneComponent.h"
#include "CoreUObject/GameFrameWork/Camera.h"

AActor::AActor()
	: UObject()
	, Owner(nullptr)
{
	RootComponent = AddComponent<USceneComponent>(this);
}

void AActor::Tick(float TickTime)
{
	// RootComponent 기준으로 Child 불러서 업데이트.
	RootComponent->TickComponent(TickTime);

	for (UActorComponent* Comp : OwnedComponent)
	{
		if (Comp == nullptr)
		{
			continue;
		}
		Comp->TickComponent(TickTime);
	}
}

void AActor::Destroy()
{
	if (RootComponent)
	{
		RootComponent->Destroy();
		FObjectFactory::DestroyObject(RootComponent);
	}
	OwnedComponent.clear();
}

void AActor::SetOwner(AActor* InOwner)
{
	Owner = InOwner;
}

FVector AActor::GetActorLocation() const
{
	return RootComponent->GetComponentLocation();
}

FRotator AActor::GetActorRotation() const
{
	return RootComponent->GetComponentRotation();
}

FVector AActor::GetActorScale() const
{
	return RootComponent->GetComponentScale();
}

void AActor::SetActorLocation(const FVector& InLocation)
{
	RootComponent->SetRelativeLocation(InLocation);
}

void AActor::SetActorRotation(const FRotator& InRotation)
{
	RootComponent->SetRelativeRotation(InRotation);
}

void AActor::SetActorScale(const FVector& InScale)
{
	RootComponent->SetRelativeScale(InScale);
}

FBoundingBox AActor::GetAABB() const {
	FVector min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (UActorComponent* comp: OwnedComponent) {
		FBoundingBox compAABB = comp->GetAABB();
		if ( compAABB.min.X < min.X ) min.X = compAABB.min.X;
		if ( compAABB.min.Y < min.Y ) min.Y = compAABB.min.Y;
		if ( compAABB.min.Z < min.Z ) min.Z = compAABB.min.Z;
		if ( compAABB.max.X > max.X ) max.X = compAABB.max.X;
		if ( compAABB.max.Y > max.Y ) max.Y = compAABB.max.Y;
		if ( compAABB.max.Z > max.Z ) max.Z = compAABB.max.Z;
	}
	return FBoundingBox(min, max);
}

