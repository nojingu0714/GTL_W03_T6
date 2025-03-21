#include "pch.h"
#include "Cube.h"
#include "Components/CubeComponent.h"

#include "Components/SphereComponent.h"

ACube::ACube()
	: AActor()
{
	CubeComponent = AddComponent<UCubeComponent>(this);
	RootComponent = CubeComponent;
	
	//CubeComponent = AddComponent<UCubeComponent>(this, FVector(), FRotator(0.0f, -45.f, 0.f), FVector(1.0f, 1.0f, 2.0f));
	//CubeComponent->SetupAttachment(RootComponent);

	//CubeComponent2 = AddComponent<UCubeComponent>(this, FVector(0.0f, 5.0f, 2.f), FRotator(0.0f, 0.0f, 45.0f), FVector(1.0f, 2.0f, 2.0f));
	//CubeComponent2->SetupAttachment(CubeComponent);
}

void ACube::Tick(float TickTime)
{
	AActor::Tick(TickTime);

	//FRotator Rotation = GetActorRotation();
	//SetActorLocation(GetActorLocation() + FVector(0.0f, 0.1f, 0.0f));
	//SetActorRotation(Rotation + FRotator(1.0f, 0.0f, 0.0f));
	//SetActorLocation(GetActorLocation()  + FVector(0.0f, 0.1f, 0.0f));
	

}

void ACube::Destroy()
{
	AActor::Destroy();
}
