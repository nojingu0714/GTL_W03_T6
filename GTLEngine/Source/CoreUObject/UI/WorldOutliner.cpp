#include "pch.h"
#include "WorldOutliner.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Editor/EditorManager.h"
#include "CoreUObject/GameFrameWork/Camera.h"
#include "Utils/Math/Geometry.h"

#include "GameFrameWork/Shapes/Triangle.h"
#include "GameFrameWork/Shapes/Sphere.h"
#include "GameFrameWork/Shapes/Cube.h"
#include "GameFrameWork/Shapes/Cylinder.h"
#include "GameFrameWork/Shapes/Cone.h"

#include "Utils/GTLStringLibrary.h"
#include "Asset/SceneManager.h"
#include <functional>


void UWorldOutliner::Tick(float TickTime) {
    ImGui::Begin("Scene Manager");

    ActorSpawner();

    ImGui::Separator();

    DebugLineToggle();

    ImGui::Separator();

    SceneHierarchy();

    ImGui::End();

}

void UWorldOutliner::ActorSpawner()
{
    if (ImGui::Button("New Scene"))
    {
        FSceneManager::NewScene();
    }


    if (ImGui::Button("Load Scene"))
    {
        FSceneManager::LoadScene("Woojae");
    }


    if (ImGui::Button("Save Scene"))
    {
        FSceneManager::SaveScene("Woojae");
    }

    const char* items[] = { "Triangle", "Sphere", "Cube", "Cylinder", "Cone" };

    if (SpawnNum < 1)
        SpawnNum = 1;

    ImGui::DragFloat3("Location", SpawnLocation);
	FVector Location(SpawnLocation[0], SpawnLocation[1], SpawnLocation[2]);
    ImGui::DragFloat3("Rotation", SpawnRotation);
	FRotator Rotation(SpawnRotation[0], SpawnRotation[1], SpawnRotation[2]);
    ImGui::DragFloat3("Scale", SpawnScale);
	FVector Scale(SpawnScale[0], SpawnScale[1], SpawnScale[2]);
    ImGui::Combo("Primitive", &CurrentPrimitiveType, items, ARRAYSIZE(items));
    int adjustedType = CurrentPrimitiveType + 2;
    ImGui::InputInt("Number of Spawn", &SpawnNum);
    
    if (ImGui::Button("Spawn")) {
        UWorld* World = UEngine::GetEngine().GetWorld();
        for (int i = 0; i < SpawnNum; i++) {
            switch (static_cast<EPrimitiveType>(adjustedType)) {
            case EPrimitiveType::Triangle:
                World->SpawnActor<ATriangle>(TEXT("Triangle"), Location, Rotation, Scale);
                break;
            case EPrimitiveType::Sphere:
                World->SpawnActor<ASphere>(TEXT("Sphere"), Location, Rotation, Scale);
                break;
            case EPrimitiveType::Cube:
                World->SpawnActor<ACube>(TEXT("Cube"), Location, Rotation, Scale);
                break;
            case EPrimitiveType::Cylinder:
                World->SpawnActor<ACylinder>(TEXT("Cylinder"), Location, Rotation, Scale);
                break;
            case EPrimitiveType::Cone:
                World->SpawnActor<ACone>(TEXT("Cone"), Location, Rotation, Scale);
                break;
            default:
                break;
            }
        }
    }
}

void UWorldOutliner::DebugLineToggle()
{
    ImGui::Checkbox("Spawn debug line", &DebugSpawnLine);
}

void UWorldOutliner::SceneHierarchy()
{
    ImGui::BeginChild("ScrollingRegion");

	UWorld* World = UEngine::GetEngine().GetWorld();
	if (!World)
		return;

    // create node for child
    std::function<void(USceneComponent*)> createNode = [&createNode](USceneComponent* comp)->void {

        std::string Name;

		if (comp == comp->GetOwner()->GetRootComponent())
			Name = UGTLStringLibrary::WStringToString(comp->GetOwner()->GetName());
        else
            Name = UGTLStringLibrary::WStringToString(comp->GetName());

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
        if (comp->GetAllChildren().size() == 0)
            flags |= ImGuiTreeNodeFlags_Leaf;

        bool nodeOpen = ImGui::TreeNodeEx(Name.c_str(), flags);

        // 노드 클릭 시 특정 동작 실행 (예: 로그 출력, 선택 처리 등)
        if (ImGui::IsItemClicked()) {
            // 원하는 동작을 여기에 추가합니다.
            // 예: 선택한 컴포넌트의 이름 출력
			UE_LOG(LogTemp, Warning, TEXT("Click Actor : %s"), comp->GetOwner()->GetName().c_str());
			UEngine::GetEngine().GetEditorManager()->SetSelectedActor( comp->GetOwner());
        }

        // 노드가 열렸다면 하위 노드 처리
        if (nodeOpen) {
            for (auto& child : comp->GetAllChildren()) {
                createNode(child);
            }
            ImGui::TreePop();
        }
    };

	for (auto& actor : World->GetActors()) {
		createNode(actor->GetRootComponent());
	}

    ImGui::IsItemClicked();

    ImGui::EndChild();
}

void UWorldOutliner::Destroy()
{
    Super::Destroy();
}
