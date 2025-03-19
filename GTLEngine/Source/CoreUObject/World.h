#pragma once

#include "Object.h"

#include "Core/Engine/Engine.h"
#include "Components/SceneComponent.h"
#include "ObjectFactory.h"

class AActor;
class ACamera;

struct FWorldCachedViewInfo
{
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix ViewProjectionMatrix;
};

class UWorld : public UObject
{

public:
	static UWorld* CreateWorld();

	template<typename T>
	T* SpawnActor(std::wstring InName, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale, AActor* InOwner)
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

			UE_LOG(TEXT("Spawn %s"), InName.c_str());
		}
		return NewObject;
	}

public:
	virtual void Tick(float TickTime) override;
	virtual void Destroy() override;

public:
	TArray<AActor*> GetActors() const { return ActiveActors; }
	ACamera* GetCamera() const { return MainCamera; }

public:
	void InitViewInfo();
	FMatrix GetViewMatrix() const { return CachedViewInfo.ViewMatrix; }
	FMatrix GetProjectionMatrix() const { return CachedViewInfo.ProjectionMatrix; }
	FMatrix GetViewProjectionMatrix() const { return CachedViewInfo.ViewProjectionMatrix; }

	void SetViewMatrix(const FMatrix& InViewMatrix) { CachedViewInfo.ViewMatrix = InViewMatrix; }
	void SetProjectionMatrix(const FMatrix& InProjectionMatrix) { CachedViewInfo.ProjectionMatrix = InProjectionMatrix; }
	void SetViewProjectionMatrix(const FMatrix& InViewProjectionMatrix) { CachedViewInfo.ViewProjectionMatrix = InViewProjectionMatrix; }


private:
	FWorldCachedViewInfo CachedViewInfo;

private:
	TArray<AActor*> ActiveActors;
	ACamera* MainCamera;
};

