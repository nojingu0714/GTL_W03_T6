#pragma once

#include "Core/Engine/Engine.h"
#include "CoreUObject/Object.h"
#include "CoreUObject/Components/SceneComponent.h"
#include "ObjectFactory.h"

class UActorComponent;
class USceneComponent;
struct FBoundingBox;
	
class AActor : public UObject
{
	DECLARE_CLASS(AActor, UObject)
public:
	AActor();

public:
	virtual void Tick(float TickTime);
	virtual void Destroy() override;

public:
	template<typename T>
	T* AddComponent(AActor* Owner, const FVector& InRelativeLocation = FVector::ZeroVector, const FRotator& InRelativeRotation = FRotator::ZeroRotator, const FVector& InRelativeScale = FVector::OneVector);

	template<typename T>
	T* GetComponentByClass() const
	{
		T* FoundComponent = nullptr;
		for (UActorComponent* Comp : OwnedComponent)
		{
			if (Comp == nullptr)
			{
				continue;
			}

			FoundComponent = Cast<T>(Comp);
			if (FoundComponent != nullptr)
			{
				return FoundComponent;
			}
		}
		return FoundComponent;
	}

public:
	USceneComponent* GetRootComponent() const { return RootComponent; }
	void SetOwner(AActor* InOwner);

	FVector GetActorLocation() const;
	FRotator GetActorRotation() const;
	FVector GetActorScale() const;

	void SetActorLocation(const FVector& InLocation);
	void SetActorRotation(const FRotator& InRotation);
	void SetActorScale(const FVector& InScale);

	FBoundingBox GetAABB() const;
	bool IsSelected = false;

protected:
	USceneComponent* RootComponent = nullptr;

public:
	TArray<UActorComponent*>& GetOwnedComponent() { return OwnedComponent; }

private:
	AActor* Owner;
	TArray<UActorComponent*> OwnedComponent;

};

template<typename T>
inline T* AActor::AddComponent(AActor* Owner, const FVector& InRelativeLocation, const FRotator& InRelativeRotation, const FVector& InRelativeScale)
{
	T* NewComp = FObjectFactory::ConstructObject<T>();

	// ActorComponent 하위 클래스가 아니라면 에러.
	UActorComponent* NewActorComp = Cast<UActorComponent>(NewComp);
	if (Cast<UActorComponent>(NewComp) == nullptr)
	{
		// "Actor Component 아니라는 에러 메시지 출력. "
		//ULogManager::AddLog(L"AddComponent: T is not UActorComponent");
		UE_LOG(LogTemp, Warning, TEXT("AddComponent: T is not UActorComponent"));
		return nullptr;
	}
	NewActorComp->SetOwner(Owner);

	USceneComponent* NewSceneComp = Cast<USceneComponent>(NewComp);
	if (NewSceneComp == nullptr)
	{
		//ULogManager::AddLog(L"AddComponent: %S", NewActorComp->GetName().c_str());
		UE_LOG(LogTemp, Warning, TEXT("AddComponent: %s"), NewActorComp->GetName().c_str());
		return nullptr;
	}

	if (RootComponent == nullptr)
	{
		RootComponent = NewSceneComp;
	}
	else
	{
		NewSceneComp->SetupAttachment(RootComponent);
	}
	NewSceneComp->SetRelativeLocation(InRelativeLocation);
	NewSceneComp->SetRelativeRotation(InRelativeRotation);
	NewSceneComp->SetRelativeScale(InRelativeScale);

	OwnedComponent.push_back(NewComp);

	// ULogManager::AddLog(L"AddComponent: %S", NewSceneComp->GetName().c_str());
	UE_LOG(Log, Warning, TEXT("AddComponent: %s"), NewActorComp->GetName().c_str());
	return NewComp;
}
