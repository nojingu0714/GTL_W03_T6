#include "pch.h"
#include "Line.h"
#include "CoreUObject/Components/LineComponent.h"

ALine::ALine() {
	LineComponent = AddComponent<ULineComponent>(this);
}

void ALine::SetRay(FRay ray) {
	float Length = ray.Direction.Length();
	LineComponent = AddComponent<ULineComponent>(
		this,
		ray.Origin,
		FRotator::ZeroRotator,
		FVector(Length, 0.f, 0.f)
	);
	LineComponent->SetDirection(FMatrix::MakeFromDirection(ray.Direction, FVector::UpVector));
}

void ALine::Tick(float TickTime) {
	AActor::Tick(TickTime);
}

void ALine::Destroy() {
	AActor::Destroy();
}
