#include "pch.h"
#include "World.h"

#include "GameFrameWork/Actor.h"
#include "GameFrameWork/Camera.h"

#include "Engine.h"
#include "Core/Gizmo/GizmoManager.h"

#include "GameFrameWork/StaticMeshActor.h"

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
	/*IClickable::GetClickableList().clear();
	IDragable::GetClickableList().clear();*/
}
