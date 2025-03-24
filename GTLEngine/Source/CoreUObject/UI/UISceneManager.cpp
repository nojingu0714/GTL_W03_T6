#include "pch.h"
#include "UISceneManager.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Core/Input/InputManager.h"
#include "Core/Gizmo/GizmoManager.h"
#include "CoreUObject/GameFrameWork/Camera.h"
#include "Utils/Math/Geometry.h"

#include "GameFrameWork/Shapes/Triangle.h"
#include "GameFrameWork/Shapes/Sphere.h"
#include "GameFrameWork/Shapes/Cube.h"
#include "GameFrameWork/Shapes/Cylinder.h"
#include "GameFrameWork/Shapes/Cone.h"
#include "GameFrameWork/Shapes/Line.h"

#include <functional>

void UUISceneManager::DeleteActor(uint32 uuid) {

}

void UUISceneManager::DeleteActor(FString InName) {

}

void UUISceneManager::Tick(float TickTime) {
    ImGui::Begin("Scene Manager");

    ActorSpawner();

    ImGui::Separator();

    DebugLineToggle();

    ImGui::Separator();

    SceneHierarchy();

    ImGui::End();

    //DebugLine 생성
    UInputManager* inputManager = UEngine::GetEngine().GetInputManager();
    if ( DebugSpawnLine && inputManager->GetMouseDown(UInputManager::EMouseButton::LEFT) ) {
		float mouse_x = inputManager->GetMouseNdcX();
		float mouse_y = inputManager->GetMouseNdcY();
		FRay ray = Geometry::CreateRayWithMouse(mouse_x, mouse_y);
		ALine* line = UEngine::GetEngine().GetWorld()->SpawnActor<ALine>(
			TEXT("Line"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			FVector::OneVector,
			nullptr
		);
        line->SetRay(ray);
	}
}

void UUISceneManager::ActorSpawner()
{
    const char* items[] = { "Triangle", "Sphere", "Cube", "Cylinder", "Cone" };

    if (SpawnNum < 1)
        SpawnNum = 1;

    ImGui::DragFloat3("Location", SpawnLocation);
    ImGui::DragFloat3("Rotation", SpawnRotation);
    ImGui::DragFloat3("Scale", SpawnScale);
    ImGui::Combo("Primitive", &CurrentPrimitiveType, items, ARRAYSIZE(items));
    int adjustedType = CurrentPrimitiveType + 2;
    ImGui::InputInt("Number of Spawn", &SpawnNum);
    if (ImGui::Button("Spawn")) {
        for (int i = 0; i < SpawnNum; i++) {
            switch (static_cast<EPrimitiveType>(adjustedType)) {
            case EPrimitiveType::Triangle:
                SpawnActor<ATriangle>(TEXT("Triangle"));
                break;
            case EPrimitiveType::Sphere:
                SpawnActor<ASphere>(TEXT("Sphere"));
                break;
            case EPrimitiveType::Cube:
                SpawnActor<ACube>(TEXT("Cube"));
                break;
            case EPrimitiveType::Cylinder:
                SpawnActor<ACylinder>(TEXT("Cylinder"));
                break;
            case EPrimitiveType::Cone:
                SpawnActor<ACone>(TEXT("Cone"));
                break;
            default:
                break;
            }
        }
    }
}

void UUISceneManager::DebugLineToggle()
{
    ImGui::Checkbox("Spawn debug line", &DebugSpawnLine);
}

void UUISceneManager::SceneHierarchy()
{
    ImGui::Checkbox("Show Hierachy", &bShowActorList);
    if ( !bShowActorList )
        return;
    ImGui::BeginChild("ScrollingRegion");

    // create node for child
    std::function<void(USceneComponent*)> createNode = [&createNode](USceneComponent* comp)->void {
        FString ws = comp->GetName();
        std::string s;
        s.assign(ws.begin(), ws.end());

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
        if (comp->GetAllChildren().size() == 0)
            flags |= ImGuiTreeNodeFlags_Leaf;

        if ( ImGui::TreeNodeEx(s.c_str(), flags) ) {
            for ( auto& child : comp->GetAllChildren() ) {
                createNode(child);
            }
            ImGui::TreePop();
        }


        };
    for (AActor* obj : UEngine::GetEngine().GetWorld()->GetActors()) {
        // cast가 안먹힌다
        if ( dynamic_cast<ALine*>(obj) != nullptr )
            continue;
        if (obj) {
            FString ws = obj->GetName();
            std::string s;
            s.assign(ws.begin(), ws.end());
            if (ImGui::TreeNodeEx(s.c_str())) {
                createNode(obj->GetRootComponent());
                ImGui::TreePop();
            }

        }
    }
    ImGui::EndChild();
}

void UUISceneManager::Destroy() {}
