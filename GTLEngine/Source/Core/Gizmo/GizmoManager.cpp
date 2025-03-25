#include "pch.h"
#include "GizmoManager.h"

#include "Gizmo/GizmoTranslate.h"
#include "Gizmo/GizmoRotate.h"
#include "Gizmo/GizmoScale.h"

#include "Input/InputManager.h"

#include "Engine.h"
#include "World.h"
#include "GameFrameWork/Camera.h"
#include "GameFrameWork/Actor.h"

#include "DirectXMath.h"
#include "Core/Resource/Types.h"

#include "Utils/Math/Geometry.h"

UGizmoManager::UGizmoManager()
	: SelectedActor(nullptr), Gizmo()
{
}

void UGizmoManager::Tick(float DeltaTime)
{
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();
	for ( auto& g : Gizmo )
		g->Tick(DeltaTime);
	Picking();
}

void UGizmoManager::Destroy()
{
}

// TODO: it should be in Inputmanager
void UGizmoManager::Picking() {
	UInputManager* inputManager = UEngine::GetEngine().GetInputManager();
	if (inputManager->GetKeyDown(VK_SPACE))
	{
		auto& mode = UEngine::GetEngine().GizmoModeIndex;
		mode = static_cast<EGizmoModeIndex>((static_cast<int>(mode) + 1) % 3);
	}
	if ( inputManager->GetMouseDown(UInputManager::EMouseButton::LEFT) ) {
		FWindowInfo winInfo = UEngine::GetEngine().GetWindowInfo();
		// test pick
		float mouse_x = inputManager->GetMouseNdcX();
		float mouse_y = inputManager->GetMouseNdcY();
		IClickable* picked = PickClickable(mouse_x, mouse_y);

		IDragable* pickedDragable;
		if ( picked && (pickedDragable = dynamic_cast<IDragable*>(picked)) ) {
			DragTarget = pickedDragable;
		}

		// if gizmo picked
		UGizmoBase* pickedGizmo;
		if ( picked && (pickedGizmo = dynamic_cast<UGizmoBase*>(picked)) ) {
			return;
		}


		// release pick
		for ( auto& clickable: Gizmo ) {
			clickable->OnRelease(static_cast<int>(mouse_x), static_cast<int>(mouse_y));
		}
		for ( auto& clickable : UEngine::GetEngine().GetWorld()->GetActors() ) {
			clickable->OnRelease(static_cast<int>(mouse_x), static_cast<int>(mouse_y));
		}
		ClearSelected();


		// if actor picked
		AActor* pickedActor;
		if ( picked && (pickedActor = dynamic_cast<AActor*>(picked)) ) {
			picked->OnClick(static_cast<int>(mouse_x), static_cast<int>(mouse_y));
			SelectedActor = pickedActor;
			AttachGizmo(pickedActor);
			return;
		}
	} else if ( inputManager->GetMouseUp(UInputManager::EMouseButton::LEFT) ) {
		DragTarget = nullptr;

	} else if ( inputManager->GetMouseButton(UInputManager::EMouseButton::LEFT) ) {
		if ( DragTarget ) {
			int mouse_x = inputManager->GetMouseClientX();
			int mouse_y = inputManager->GetMouseClientY();
			int mouse_dx = inputManager->GetMouseDeltaX();
			int mouse_dy = inputManager->GetMouseDeltaY();
			DragTarget->OnDragTick(mouse_x, mouse_y, mouse_dx, mouse_dy);
		}
	}
}


IClickable* UGizmoManager::PickClickable(float MouseX, float MouseY) const {
	FRay ray = Geometry::CreateRayWithMouse(MouseX, MouseY);
	AActor* camera = UEngine::GetEngine().GetWorld()->GetCamera();
	TArray<AActor*> actors = UEngine::GetEngine().GetWorld()->GetActors();
	TArray<IClickable*> clickables = TArray<IClickable*>();
	IClickable* selected = nullptr;
	UGizmoBase* selectedGizmo = nullptr;

	clickables.insert(clickables.end(), Gizmo.begin(), Gizmo.end());
	clickables.insert(clickables.end(), actors.begin(),actors.end());
	float minDistancePow = FLT_MAX;
	float gizmoMinDistancePow = FLT_MAX;

	FVector hitpoint;
	for (IClickable* clickable: clickables) {
		if ( camera && clickable->IsClicked(ray, 100.f, hitpoint))
		{
			float distanceSq = (camera->GetActorLocation() - hitpoint).LengthSquared();
			if (UGizmoBase* gizmo = dynamic_cast<UGizmoBase*>(clickable)) {
				if (gizmo->GizmoMode != UEngine::GetEngine().GizmoModeIndex)
					continue;
				if (distanceSq < gizmoMinDistancePow) {
					gizmoMinDistancePow = distanceSq;
					selectedGizmo = gizmo;
				}
			}
			else {
				if (distanceSq < minDistancePow)
				{
					minDistancePow = distanceSq;
					selected = clickable;
				}
			}
		}
	}

	if (selectedGizmo)
		return selectedGizmo;
	else
		return selected;
}

void UGizmoManager::ClearSelected() {
	if (SelectedActor) {
		SelectedActor->IsSelected = false;
		SelectedActor = nullptr;
	}
	for ( auto& g : Gizmo )
		delete g;
	Gizmo.clear();
}

const TArray<UGizmoBase*> UGizmoManager::GetGizmo() {
	return Gizmo;
}

void UGizmoManager::AttachGizmo(AActor* target) {
	return;
	UGizmoTranslate* GizmoTranslate;
	GizmoTranslate = FObjectFactory::ConstructObject<UGizmoTranslate>();
	GizmoTranslate->Init(UGizmoBase::EAxis::X, target);
	Gizmo.push_back(GizmoTranslate);
	GizmoTranslate = FObjectFactory::ConstructObject<UGizmoTranslate>();
	GizmoTranslate->Init(UGizmoBase::EAxis::Y, target);
	Gizmo.push_back(GizmoTranslate);
	GizmoTranslate = FObjectFactory::ConstructObject<UGizmoTranslate>();
	GizmoTranslate->Init(UGizmoBase::EAxis::Z, target);
	Gizmo.push_back(GizmoTranslate);

	UGizmoRotate* GizmoRotate;
	GizmoRotate = FObjectFactory::ConstructObject<UGizmoRotate>();
	GizmoRotate->Init(UGizmoBase::EAxis::X, target);
	Gizmo.push_back(GizmoRotate);
	GizmoRotate = FObjectFactory::ConstructObject<UGizmoRotate>();
	GizmoRotate->Init(UGizmoBase::EAxis::Y, target);
	Gizmo.push_back(GizmoRotate);
	GizmoRotate = FObjectFactory::ConstructObject<UGizmoRotate>();
	GizmoRotate->Init(UGizmoBase::EAxis::Z, target);
	Gizmo.push_back(GizmoRotate);

	UGizmoScale* GizmoScale;
	GizmoScale = FObjectFactory::ConstructObject<UGizmoScale>();
	GizmoScale->Init(UGizmoBase::EAxis::X, target);
	Gizmo.push_back(GizmoScale);
	GizmoScale = FObjectFactory::ConstructObject<UGizmoScale>();
	GizmoScale->Init(UGizmoBase::EAxis::Y, target);
	Gizmo.push_back(GizmoScale);
	GizmoScale = FObjectFactory::ConstructObject<UGizmoScale>();
	GizmoScale->Init(UGizmoBase::EAxis::Z, target);
	Gizmo.push_back(GizmoScale);
}