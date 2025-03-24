#pragma once
#include "CoreUObject/UI/UIBase.h"
#include "CoreUObject/World.h"
#include "Core/Engine/Engine.h"

class UUISceneManager: public UUIBase {
public:
	template<typename T>
	T* SpawnActor(FString InName) {
		T* actor = UEngine::GetEngine().GetWorld()->SpawnActor<T>(
			InName,
			FVector(SpawnLocation[0], SpawnLocation[1], SpawnLocation[2]),
			FRotator(SpawnRotation[0], SpawnRotation[1], SpawnRotation[2]),
			FVector(SpawnScale[0], SpawnScale[1], SpawnScale[2]),
			nullptr
		);
		return actor;
	}
	void DeleteActor(uint32 uuid);
	void DeleteActor(FString InName);

	void ShowActorList();

	// UUIBase을(를) 통해 상속됨
	void Tick(float TickTime) override;
	void ActorSpawner();
	void DebugLineToggle();
	void SceneHierarchy();
	void Destroy() override;

private:
	float SpawnLocation[3];
	float SpawnRotation[3];
	float SpawnScale[3] = { 1.f, 1.f, 1.f };
	int32 CurrentPrimitiveType;
	int32 SpawnNum;
	bool DebugSpawnLine = false;
	bool bShowActorList = true;
};
