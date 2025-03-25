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

FGizmoManager::FGizmoManager()
{
}

void FGizmoManager::Init(UDirectXHandle* Handle)
{
	// Gizmo를 위한 Primitive 생성
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
