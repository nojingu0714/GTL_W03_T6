#include "pch.h"
#include "PropertyPanel.h"

#include "ImGui/imgui.h"
#include "Utils/Math/Vector.h"
#include "Engine.h"

#include "Editor/EditorManager.h"

#include "GameFrameWork/Actor.h"
#include "GameFrameWork/StaticMeshActor.h"

#include "Asset/AssetManager.h"
#include "Asset/ObjManager.h"
#include "Utils/GTLStringLibrary.h"


UPropertyPanel::UPropertyPanel()
	:UUIBase(), WindowWidth(280.f), WindowHeight(360.f)
{
}

void UPropertyPanel::Tick(float DeltaTime)
{
	float scaleX = io.DisplaySize.x / 1600.0f;
	float scaleY = io.DisplaySize.y / 900.0f;

	ImVec2 WinSize(WindowWidth * scaleX, WindowHeight * scaleY);

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - WinSize.x - 5, 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(WinSize, ImGuiCond_Appearing);
	ImGui::Begin("Property Panel", nullptr, ImGuiWindowFlags_NoResize);

	WorldCoordCheckbox();

	ImGui::Separator();

	PropertiesInspector();

	ImGui::End();
}

void UPropertyPanel::Destroy()
{
}

void UPropertyPanel::WorldCoordCheckbox()
{
	//ImGui::Checkbox("WorldCoord", &AGizmoBase::IsAbsoluteCoord);
}

void UPropertyPanel::PropertiesInspector()
{
	AActor* SelectedActor = UEngine::GetEngine().GetEditorManager()->GetSelectedActor();

	if (SelectedActor) {
		Location[0] = SelectedActor->GetActorLocation().X;
		Location[1] = SelectedActor->GetActorLocation().Y;
		Location[2] = SelectedActor->GetActorLocation().Z;
		ImGui::DragFloat3("Location", Location);
		SelectedActor->SetActorLocation(FVector(Location[0], Location[1], Location[2]));

		Rotation[0] = SelectedActor->GetActorRotation().Euler().X;
		Rotation[1] = SelectedActor->GetActorRotation().Euler().Y;
		Rotation[2] = SelectedActor->GetActorRotation().Euler().Z;
		ImGui::DragFloat3("Rotation", Rotation);
		SelectedActor->SetActorRotation(FRotator(Rotation[1], Rotation[2], Rotation[0]));

		Scale[0] = SelectedActor->GetActorScale().X;
		Scale[1] = SelectedActor->GetActorScale().Y;
		Scale[2] = SelectedActor->GetActorScale().Z;
		ImGui::DragFloat3("Scale", Scale);
		SelectedActor->SetActorScale(FVector(Scale[0], Scale[1], Scale[2]));

		ImGui::Text("GUID : %d", SelectedActor->GetUUID());

		DrawSMActorProperties(SelectedActor);

	}
}

void UPropertyPanel::DrawSMActorProperties(AActor* SelectedActor)
{
	if (AStaticMeshActor* SMActor = Cast<AStaticMeshActor>(SelectedActor))
	{
		TArray<FString> MeshNames = FAssetManager::Get().GetObjFileNames();

		// Array로 넣어줘야 수명이 맞음.
		TArray<std::string> UTF8MeshNames;
		TArray<const char*> MeshItems;
		for (int i = 0; i < MeshNames.size(); i++)
		{
			UTF8MeshNames.push_back(UGTLStringLibrary::WStringToString(MeshNames[i]));
			MeshItems.push_back(UTF8MeshNames[i].c_str());
		}

		static int32 CurrentIndex = 0;
		if (ImGui::Combo("Mesh", &CurrentIndex, MeshItems.data(), MeshItems.size()))
		{
			UStaticMesh* Mesh = FObjManager::LoadObjStaticMesh(MeshNames[CurrentIndex]);
			SMActor->SetStaticMesh(Mesh);
		}

	}
}
