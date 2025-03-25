#include "pch.h"
#include "GizmoManager.h"

#include "Input/InputManager.h"

#include "Engine.h"
#include "World.h"
#include "GameFrameWork/Camera.h"
#include "GameFrameWork/Actor.h"

#include "DirectXMath.h"
#include "Core/Resource/Types.h"

#include "Utils/Math/Geometry.h"

UGizmoManager::UGizmoManager()
{
}

void UGizmoManager::Tick(float DeltaTime)
{
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();
	/*for ( auto& g : Gizmo )
		g->Tick(DeltaTime);*/
	//Picking();
}

void UGizmoManager::Destroy()
{
}
