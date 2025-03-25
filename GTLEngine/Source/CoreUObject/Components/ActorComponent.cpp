#include "pch.h"
#include "ActorComponent.h"

void UActorComponent::TickComponent(float TickTime)
{
	// TODO: Log 추가.
}

void UActorComponent::Destroy()
{
}

bool UActorComponent::IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const {
	return false;
}
