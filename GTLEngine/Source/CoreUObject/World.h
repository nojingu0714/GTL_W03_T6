#pragma once

#include "Object.h"

#include "Core/Engine/Engine.h"
#include "Components/SceneComponent.h"
#include "ObjectFactory.h"

class AActor;

class UWorld : public UObject
{

public:
	static UWorld* CreateWorld();

	template<typename T>
	T* SpawnActor(std::wstring InName, const FVector& InLocation = FVector(), const FRotator& InRotation = FRotator(), const FVector& InScale = FVector::OneVector, AActor* InOwner = nullptr)
	{
		T* NewObject = FObjectFactory::ConstructObject<T>();
		
		AActor* newActor = Cast<AActor>(NewObject);
		// newActor-> SetLocation, SetScale, SetRotation.
		newActor->SetActorLocation(InLocation);
		newActor->SetActorScale(InScale);
		newActor->SetActorRotation(InRotation);

		if (newActor != nullptr)
		{
			newActor->SetOwner(InOwner);
			
			USceneComponent* Comp = newActor->GetRootComponent();
			Comp->SetRelativeLocation(InLocation);
			Comp->SetRelativeRotation(InRotation);
			Comp->SetRelativeScale(InScale);

			ActiveActors.push_back(newActor);

			UE_LOG(LogTemp, Display, TEXT("Spawn %s"), InName.c_str());
		}
		return NewObject;
	}

public:
	virtual void Tick(float TickTime);
	virtual void Destroy() override;

public:
	TArray<AActor*> GetActors() const { return ActiveActors; }

private:
	TArray<AActor*> ActiveActors;
};

