#include "pch.h"
#include "Actor.h"

#include "Core/Engine/Engine.h"
#include "CoreUObject/World.h"
#include "Utils/Math/Geometry.h"
#include "Components/SceneComponent.h"
#include "CoreUObject/GameFrameWork/Camera.h"
#include "CoreUObject/Components/StaticMeshComponent.h"

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
	FBoundingBox Box(FVector(FLT_MAX, FLT_MAX, FLT_MAX), FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX));
	for (UActorComponent* comp : OwnedComponent) {
		if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(comp))
		{
			FBoundingBox compAABB = StaticMeshComp->GetAABB(StaticMeshComp->GetWorldMatrix());
			Box += compAABB;
		}
	}
	return Box;
}

void AActor::OnClick(int mx, int my) {
	IsSelected = true;
}

void AActor::OnRelease(int mx, int my) {
	IsSelected = false;
}

bool AActor::IsClicked(FRay ray, float maxDistance, FVector& hitpoint) {
	// aabb로 1차 검사
	FBoundingBox aabb = GetAABB();
	if ( !Geometry::IsRayIntersectAABB(aabb, ray, 100.f) ) {
		return false;
	}
	else
	{
		return true;
	}




}
