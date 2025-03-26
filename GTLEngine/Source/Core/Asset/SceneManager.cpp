#include "pch.h"
#include "SceneManager.h"

#include "GTLStringLibrary.h"

#include <fstream>
#include <sstream>
#include <filesystem>

#include "World.h"
#include "GameFrameWork/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

#include "Asset/ObjManager.h"

#include "SimpleJSON/json.hpp"

#include "Editor/EditorManager.h"
#include "Window/Viewport.h"


void FSceneManager::SaveScene(const std::string& InSceneName, const std::string& InPath)
{
    UWorld* World = UEngine::GetEngine().GetWorld();
    if (!World) return;

    json::JSON Scene;
    json::JSON Primitives = json::Object();  // Primitives 객체 생성

    for (const AActor* Actor : World->GetActors())
    {
        USceneComponent* RootComponent = Actor->GetRootComponent();
        if (!RootComponent)
            continue;

        json::JSON Primitive = json::Object();
        Primitive["Location"] = json::FVectorToJSON(RootComponent->GetComponentLocation());
        Primitive["Rotation"] = json::FRotatorToJSON(RootComponent->GetComponentRotation());
        Primitive["Scale"] = json::FVectorToJSON(RootComponent->GetComponentScale());

        if (AStaticMeshActor* SMActor = Cast<AStaticMeshActor>(Actor))
        {
            Primitive["ObjStaticMeshAsset"] = UGTLStringLibrary::WStringToString(SMActor->GetStaticMeshComponent()->GetStaticMesh()->GetAssetPathFileName());
            Primitive["Type"] = "StaticMeshComp";
        }
        else
        {
            Primitive["Type"] = UGTLStringLibrary::WStringToString(RootComponent->StaticClass()->GetName());
        }

        // ✅ Actor의 이름을 키로 사용하여 Primitives에 추가
        Primitives[UGTLStringLibrary::WStringToString(Actor->GetName())] = Primitive;
    }

    // ✅ Scene에 Primitives 추가
    Scene["Primitives"] = Primitives;

    std::string ScenePath = InPath + InSceneName + ".scene";

    if (!std::filesystem::exists(InPath))
    {
        std::filesystem::create_directories(InPath);
    }

    std::ofstream outFile(ScenePath);
    if (outFile.is_open())
    {
        outFile << Scene.dump();
        outFile.close();
        UE_LOG(LogTemp, Warning, TEXT("Scene Saved: %s"), UGTLStringLibrary::StringToWString(ScenePath));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to save: %s"), UGTLStringLibrary::StringToWString(ScenePath).c_str());
    }
}

void FSceneManager::NewScene()
{
    UEngine::GetEngine().CreateNewWorld();
}

void FSceneManager::LoadScene(const std::string& InSceneName)
{
    UEngine::GetEngine().CreateNewWorld();

    std::string ScenePath = "Contents/Scene/" + InSceneName + ".scene";

    std::ifstream inFile(ScenePath);
    if (!inFile.is_open()) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load scene file: %s"), UGTLStringLibrary::StringToWString(ScenePath));
        return;
    }

    std::string jsonData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    json::JSON SceneJSON = json::JSON::Load(jsonData);

    UWorld* World = UEngine::GetEngine().GetWorld();
    if (!World) return;

    if (SceneJSON.hasKey("Primitives")) {
        json::JSON PrimitiveJSON = SceneJSON["Primitives"];
        for (auto& pair : PrimitiveJSON.ObjectRange())
        {
            json::JSON& obj = pair.second;
            FString MeshPath = UGTLStringLibrary::StringToWString(obj["ObjStaticMeshAsset"].ToString());
            FVector Location = JSONToFVector(obj["Location"]);
            FRotator Rotation = JSONToFRotator(obj["Rotation"]);
            FVector Scale = JSONToFVector(obj["Scale"]);
            std::string Type = obj["Type"].ToString();

            if (Type == "StaticMeshComp")
            {
                AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(TEXT("MeshActor"), Location, Rotation, Scale, nullptr);
                Actor->SetStaticMesh(FObjManager::LoadObjStaticMesh(MeshPath));
            }
        }
    }

    if (SceneJSON.hasKey("PerspectiveCamera")) {
        json::JSON CameraJSON = SceneJSON["PerspectiveCamera"];
        FVector Location = JSONToFVector(CameraJSON["Location"]);
        FRotator Rotation = JSONToFRotator(CameraJSON["Rotation"]);
        float FOV = CameraJSON["FOV"].ToFloat();
        float Near = CameraJSON["NearClip"].ToFloat();
        float Far = CameraJSON["FarClip"].ToFloat();

        // 카메라 액터.

    }

    UE_LOG(LogTemp, Display, TEXT("Scene Loaded: %s"), UGTLStringLibrary::StringToWString(ScenePath).c_str());
}
