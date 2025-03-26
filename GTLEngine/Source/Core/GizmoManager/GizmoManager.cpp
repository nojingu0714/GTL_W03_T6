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
#include "GameFrameWork/StaticMeshActor.h"

FGizmoManager::FGizmoManager()
{
}

void FGizmoManager::Init()
{
	GizmoTranslate = FObjectFactory::ConstructObject<AGizmoTranslate>();
	GizmoRotate = FObjectFactory::ConstructObject<AGizmoRotate>();
	GizmoScale = FObjectFactory::ConstructObject<AGizmoScale>();
	GizmoTranslate->Init();
	GizmoRotate->Init();
	GizmoScale->Init();
	GizmoOnDrag = GizmoScale;
}

void FGizmoManager::Tick(float DeltaTime)
{
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();
	if (InputManager->GetKeyDown(VK_SPACE)) // SPace를 누르면 모드 변경
	{
		GizmoMode = static_cast<EGizmoModeIndex>((static_cast<int>(GizmoMode) + 1) % 3);
	}
	if (GizmoOnDrag)
	{
		switch (GizmoMode)
		{
		case EGizmoModeIndex::GMI_GizmoTranslate:
			GizmoOnDrag = GizmoTranslate;
			break;
		case EGizmoModeIndex::GMI_GizmoRotate:
			GizmoOnDrag = GizmoRotate;
			break;
		case EGizmoModeIndex::GMI_GizmoScale:
			GizmoOnDrag = GizmoScale;
			break;
		default:
			break;
		}
	}

	if (GizmoOnDrag && SelectedActor)
	{
		GizmoOnDrag->SetActorLocation(SelectedActor->GetActorLocation());
		GizmoOnDrag->SetActorRotation(SelectedActor->GetActorRotation());
		GizmoOnDrag->SetActorScale(SelectedActor->GetActorScale());
	}

}

void FGizmoManager::Destroy()
{
}

// TODO: it should be in Inputmanager
void FGizmoManager::ProcessPicking(FRay Ray) {
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();


	// 좌클릭 했을 경우 : actor를 클릭했을 경우 -> 선택된 액터 변경(같으면 그냥 유지됨).
	// 만약 gizmo를 클릭했을경우(z 무시함) -> gizmo 를 활성화???
	
	// 클릭했을때의 경우의 수.
	// 1. 기즈모도 액터도 선택되지 않음.
	// 2. 기즈모만 선택되고 액터는 선택되지 않음.
	// 3. 기즈모는 선택되지 않고 액터는 선택됨.
	// 4. 기즈모랑 액터 둘다 선택됨.
	// 다시말해서,.
	// 기즈모   ||    액터.
	//   x              x.
	//   o              x.
	//   x              o.
	//   o              o.
	// 1 : 선택된 액터, 기즈모 모두 선택 해제.
	// 2 : 현재 상태 유지.
	// 3 : 선택된 액터 변경.
	// 4 : 기즈모 선택(즉, 아무것도 안함).
	//.
	// 다시 정리하면.
	// 기즈모가 선택 -> 유지.
	// 기즈모가 선택되지 않음.
	//     1 : 액터 선택되었을 때 : 액터 선택 & 기즈모 띄우기.
	//     2 : 액터 선택되지 않았을 때 : 선택 풀기.
	
	if (InputManager->GetMouseDown(UInputManager::EMouseButton::LEFT)) {
		// 기즈모가 떠있어야 할수있으니까.
		bool isGizmoIntersect = false;
		if (GizmoOnDrag)
		{
			float DistancePlaceHolder = FLT_MAX;
			switch (GizmoMode)
			{
			case EGizmoModeIndex::GMI_GizmoTranslate:
				//isGizmoIntersect = GizmoTranslate->Intersects(Ray, DistanceNearestGizmo);
				isGizmoIntersect = GizmoTranslate->Intersects(Ray, DistancePlaceHolder);
				break;
			case EGizmoModeIndex::GMI_GizmoRotate:
				isGizmoIntersect = GizmoRotate->Intersects(Ray, DistancePlaceHolder);
				break;
			case EGizmoModeIndex::GMI_GizmoScale:
				isGizmoIntersect = GizmoScale->Intersects(Ray, DistancePlaceHolder);
				break;
			default:
				break;
			}
		}
		// 기즈모가 선택되지 않았을때
		if (!isGizmoIntersect)
		{
			// 가장 앞에 있는 actor를 검출.
			AActor* ActorOnRay = nullptr;
			float Nearest = FLT_MAX;

			for (TObjectIterator<AStaticMeshActor> It; It; ++It)
			{
				float DistanceActor;
				if (It->IsClicked(Ray, DistanceActor))
				{
					if (DistanceActor < Nearest && DistanceActor > 0)
					{
						ActorOnRay = *It;
						Nearest = DistanceActor;
					}
				}
			}

			// 액터가 선택되었을때.
			if (ActorOnRay)
			{
				SelectedActor->IsSelected = false;
				SelectedActor = ActorOnRay;
				SelectedActor->IsSelected = true;

				switch (GizmoMode)
				{
				case EGizmoModeIndex::GMI_GizmoTranslate:
					GizmoOnDrag = GizmoTranslate;
					break;
				case EGizmoModeIndex::GMI_GizmoRotate:
					GizmoOnDrag = GizmoRotate;
					break;
				case EGizmoModeIndex::GMI_GizmoScale:
					GizmoOnDrag = GizmoScale;
					break;
				default:
					break;
				}
			}
			// 액터가 선택되지 않았을 때.
			else
			{
				if (SelectedActor) {
					SelectedActor->IsSelected = false;
					SelectedActor = nullptr;
					GizmoOnDrag = nullptr;
				}
			}
		}
		// 기즈모가 선택되었을때는 아무것도 안함
		else {}
	}
	
	// 마우스를 뗐을때 
	/*else if (InputManager->GetMouseUp(UInputManager::EMouseButton::LEFT)) {
		GizmoOnDrag = nullptr;
	}*/
	
	// 누르고 있을경우
	else if (InputManager->GetMouseButton(UInputManager::EMouseButton::LEFT)) {
		if (GizmoOnDrag)
		{
			switch (GizmoMode)
			{
			case EGizmoModeIndex::GMI_GizmoTranslate:
				//GizmoTranslate->OnDragTick();
				break;
			case EGizmoModeIndex::GMI_GizmoRotate:
				//GizmoRotate->OnDragTick();
				break;
			case EGizmoModeIndex::GMI_GizmoScale:
				//GizmoScale->OnDragTick();
				break;
			default:
				break;
			}
		}
	}
}


void FGizmoManager::ClearSelected() {
	if (SelectedActor) {
		SelectedActor->IsSelected = false;
		SelectedActor = nullptr;
	}
}

//const TArray<AGizmoBase*> FGizmoManager::GetGizmo() {
//	return Gizmos;
//}
//
//void FGizmoManager::AttachGizmo(AActor* target) {
//	AGizmoTranslate* GizmoTranslate;
//	GizmoTranslate = FObjectFactory::ConstructObject<AGizmoTranslate>();
//	GizmoTranslate->Init(AGizmoBase::EAxis::X, target);
//	Gizmos.push_back(GizmoTranslate);
//	GizmoTranslate = FObjectFactory::ConstructObject<AGizmoTranslate>();
//	GizmoTranslate->Init(AGizmoBase::EAxis::Y, target);
//	Gizmos.push_back(GizmoTranslate);
//	GizmoTranslate = FObjectFactory::ConstructObject<AGizmoTranslate>();
//	GizmoTranslate->Init(AGizmoBase::EAxis::Z, target);
//	Gizmos.push_back(GizmoTranslate);
//
//	AGizmoRotate* GizmoRotate;
//	GizmoRotate = FObjectFactory::ConstructObject<AGizmoRotate>();
//	GizmoRotate->Init(AGizmoBase::EAxis::X, target);
//	Gizmos.push_back(GizmoRotate);
//	GizmoRotate = FObjectFactory::ConstructObject<AGizmoRotate>();
//	GizmoRotate->Init(AGizmoBase::EAxis::Y, target);
//	Gizmos.push_back(GizmoRotate);
//	GizmoRotate = FObjectFactory::ConstructObject<AGizmoRotate>();
//	GizmoRotate->Init(AGizmoBase::EAxis::Z, target);
//	Gizmos.push_back(GizmoRotate);
//
//	AGizmoScale* GizmoScale;
//	GizmoScale = FObjectFactory::ConstructObject<AGizmoScale>();
//	GizmoScale->Init(AGizmoBase::EAxis::X, target);
//	Gizmos.push_back(GizmoScale);
//	GizmoScale = FObjectFactory::ConstructObject<AGizmoScale>();
//	GizmoScale->Init(AGizmoBase::EAxis::Y, target);
//	Gizmos.push_back(GizmoScale);
//	GizmoScale = FObjectFactory::ConstructObject<AGizmoScale>();
//	GizmoScale->Init(AGizmoBase::EAxis::Z, target);
//	Gizmos.push_back(GizmoScale);
//}