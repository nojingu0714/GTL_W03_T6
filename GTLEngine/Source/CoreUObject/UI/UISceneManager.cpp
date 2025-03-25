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
        UWorld* World = UEngine::GetEngine().GetWorld();
        for (int i = 0; i < SpawnNum; i++) {
            switch (static_cast<EPrimitiveType>(adjustedType)) {
            case EPrimitiveType::Triangle:
                World->SpawnActor<ATriangle>(TEXT("Triangle"));
                break;
            case EPrimitiveType::Sphere:
                World->SpawnActor<ASphere>(TEXT("Sphere"));
                break;
            case EPrimitiveType::Cube:
                World->SpawnActor<ACube>(TEXT("Cube"));
                break;
            case EPrimitiveType::Cylinder:
                World->SpawnActor<ACylinder>(TEXT("Cylinder"));
                break;
            case EPrimitiveType::Cone:
                World->SpawnActor<ACone>(TEXT("Cone"));
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
    ImGui::EndChild();
}

void UUISceneManager::Destroy()
{
    Super::Destroy();
}
