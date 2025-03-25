#include "pch.h"
#include "GizmoManager.h"

#include "Input/InputManager.h"

#include "Engine.h"
#include "World.h"
#include "DirectX11/DirectXHandle.h"

#include "GameFrameWork/Camera.h"
#include "GameFrameWork/Actor.h"

#include "Core/Resource/Types.h"

#include "Utils/Math/Geometry.h"

#include "ObjectFactory.h"
#include "Gizmo/GizmoActor.h"

FGizmoManager::FGizmoManager()
{
}

void FGizmoManager::Init()
{
	// Gizmo를 위한 Primitive 생성

	GizmoActor = FObjectFactory::ConstructObject<AGizmoActor>();
	
}

void FGizmoManager::Tick(float DeltaTime)
{
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();
	/*for ( auto& g : Gizmo )
		g->Tick(DeltaTime);*/
	//Picking();
}

void FGizmoManager::Destroy()
{
}
