#include "pch.h"
#include "World.h"

#include "GameFrameWork/Actor.h"
#include "GameFrameWork/Camera.h"

#include "Engine.h"
#include "Core/GizmoManager/GizmoManager.h"

#include "GameFrameWork/StaticMeshActor.h"
#include "CoreUObject/gizmo/GizmoActor.h"

UWorld* UWorld::CreateWorld()
{
	UWorld* NewWorld = FObjectFactory::ConstructObject<UWorld>();

	// TODO: 나중에 분리.

    return NewWorld;
}


void UWorld::Tick(float TickTime)
{
	for (AActor* Actor : ActiveActors)
	{
		Actor->Tick(TickTime);

	}
}

void UWorld::Destroy()
{
	for (AActor* Actor : ActiveActors)
	{
		if (Actor)
		{
			Actor->Destroy();
			FObjectFactory::DestroyObject(Actor);
		}
	}
	ActiveActors.clear();
	/*AActor::GetClickableList().clear();
	IDragable::GetClickableList().clear();*/
}
