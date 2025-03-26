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

    json::JSON CameraArray = json::Array();
    for (const auto& Viewport : UEngine::GetEngine().GetEditorManager()->GetViewports())
    {
        if (FViewportCamera* Camera = Viewport.GetCamera())
        {
            json::JSON CameraJSON;
            CameraJSON["Location"] = json::FVectorToJSON(Camera->Location);
            CameraJSON["Rotation"] = json::FRotatorToJSON(Camera->Rotation);
            CameraJSON["ProjectionMode"] = static_cast<int>(Camera->ProjectionMode);
            CameraJSON["FieldOfView"] = Camera->FieldOfView;
            CameraJSON["NearClip"] = Camera->NearClip;
            CameraJSON["FarClip"] = Camera->FarClip;
            CameraJSON["Speed"] = Camera->Speed;
            CameraJSON["Sensitive"] = Camera->Sensitive;

            CameraArray.append(CameraJSON);
        }
    }

    Scene["Cameras"] = CameraArray;

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

    // ✅ 오브젝트 로드
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

    // ✅ 카메라 로드 & 적용
    if (SceneJSON.hasKey("Cameras"))
    {
        const json::JSON& CameraArray = SceneJSON["Cameras"];
        auto& Viewports = UEngine::GetEngine().GetEditorManager()->GetViewports();
        
        size_t NumCameras = std::min(static_cast<size_t>(CameraArray.length()), Viewports.size());


        for (size_t i = 0; i < NumCameras; ++i)
        {
            const json::JSON& CameraJSON = CameraArray.at(i);  // 🔥 `at(i)` 사용

            if (Viewports[i].GetCamera())
            {
                FViewportCamera* Camera = Viewports[i].GetCamera();

                if (CameraJSON.hasKey("Location")) Camera->Location = JSONToFVector(CameraJSON.at("Location"));
                if (CameraJSON.hasKey("Rotation")) Camera->Rotation = JSONToFRotator(CameraJSON.at("Rotation"));
                if (CameraJSON.hasKey("ProjectionMode")) Camera->ProjectionMode = static_cast<EProjectionMode>(CameraJSON.at("ProjectionMode").ToInt());
                if (CameraJSON.hasKey("FieldOfView")) Camera->FieldOfView = CameraJSON.at("FieldOfView").ToFloat();
                if (CameraJSON.hasKey("NearClip")) Camera->NearClip = CameraJSON.at("NearClip").ToFloat();
                if (CameraJSON.hasKey("FarClip")) Camera->FarClip = CameraJSON.at("FarClip").ToFloat();
                if (CameraJSON.hasKey("Speed")) Camera->Speed = CameraJSON.at("Speed").ToFloat();
                if (CameraJSON.hasKey("Sensitive")) Camera->Sensitive = CameraJSON.at("Sensitive").ToFloat();
            }
        }
    }




    UE_LOG(LogTemp, Display, TEXT("Scene Loaded: %s"), UGTLStringLibrary::StringToWString(ScenePath).c_str());
}
