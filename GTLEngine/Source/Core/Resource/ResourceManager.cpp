#include "pch.h"
#include "ResourceManager.h"

#include "Resource/Shape/PrimitiveShape.h"

#include <fstream>
#include <sstream>

#include "Engine.h"
#include "World.h"
#include "GameFrameWork/Actor.h"

#include "GameFrameWork/Shapes/Triangle.h"
#include "GameFrameWork/Shapes/Sphere.h"
#include "GameFrameWork/Shapes/Cube.h"
#include "GameFrameWork/Shapes/Cylinder.h"
#include "GameFrameWork/Shapes/Cone.h"

UResourceManager::UResourceManager()
{


}

UResourceManager::~UResourceManager()
{
    Release();
}

//void UResourceManager::LoadTranslateGizmos()
//{
//    ObjData Obj = LoadObj(L"Resource/Shape/GizmoTranslate.obj");
//
//    TArray<FVertexPNCT> XVertices;
//    TArray<FVertexPNCT> YVertices;
//    TArray<FVertexPNCT> ZVertices;
//
//    for (size_t i = 0; i < Obj.vertices.size(); ++i)
//    {
//        FVertexSimple xVert = Obj.vertices[i];
//        FVertexSimple yVert;
//        yVert.X = xVert.Y; yVert.Y = xVert.X; yVert.Z = xVert.Z; //Y축 방향
//        FVertexSimple zVert;
//        zVert.X = xVert.Z; zVert.Y = xVert.Y; zVert.Z = xVert.X; //Z축 방향
//        // X축은 빨간색 (1,0,0)
//        xVert.R = 1.f; xVert.G = 0.f; xVert.B = 0.f;
//        // Y축은 녹색 (0,1,0)
//        yVert.R = 0.f; yVert.G = 1.f; yVert.B = 0.f;
//        // Z축은 파란색 (0,0,1)
//        zVert.R = 0.f; zVert.G = 0.f; zVert.B = 1.f;
//
//        XVertices.push_back(xVert);
//        YVertices.push_back(yVert);
//        ZVertices.push_back(zVert);
//    }
//
//    GizmoVertexDataMap[EGizmoViewType::XTranslate] = XVertices;
//    GizmoVertexDataMap[EGizmoViewType::YTranslate] = YVertices;
//    GizmoVertexDataMap[EGizmoViewType::ZTranslate] = ZVertices;
//    GizmoIndexDataMap[EGizmoViewType::XTranslate] = Obj.indices;
//    GizmoIndexDataMap[EGizmoViewType::YTranslate] = Obj.indices;
//    GizmoIndexDataMap[EGizmoViewType::ZTranslate] = Obj.indices;
//}
//
//void UResourceManager::LoadRotateGizmos()
//{
//    ObjData Obj = LoadObj(L"Resource/Shape/GizmoRotate.obj");
//
//    TArray<FVertexSimple> XVertices;
//    TArray<FVertexSimple> YVertices;
//    TArray<FVertexSimple> ZVertices;
//
//    for (size_t i = 0; i < Obj.vertices.size(); ++i)
//    {
//        FVertexSimple xVert = Obj.vertices[i];
//        FVertexSimple yVert;
//        yVert.X = xVert.Y; yVert.Y = xVert.X; yVert.Z = xVert.Z; //Y축 방향
//        FVertexSimple zVert;
//        zVert.X = xVert.Z; zVert.Y = xVert.Y; zVert.Z = xVert.X; //Z축 방향
//
//        // X축은 빨간색 (1,0,0)
//        xVert.R = 1.f; xVert.G = 0.f; xVert.B = 0.f;
//        // Y축은 녹색 (0,1,0)
//        yVert.R = 0.f; yVert.G = 1.f; yVert.B = 0.f;
//        // Z축은 파란색 (0,0,1)
//        zVert.R = 0.f; zVert.G = 0.f; zVert.B = 1.f;
//
//        XVertices.push_back(xVert);
//        YVertices.push_back(yVert);
//        ZVertices.push_back(zVert);
//    }
//
//    GizmoVertexDataMap[EGizmoViewType::XRotate] = XVertices;
//    GizmoVertexDataMap[EGizmoViewType::YRotate] = YVertices;
//    GizmoVertexDataMap[EGizmoViewType::ZRotate] = ZVertices;
//    GizmoIndexDataMap[EGizmoViewType::XRotate] = Obj.indices;
//    GizmoIndexDataMap[EGizmoViewType::YRotate] = Obj.indices;
//    GizmoIndexDataMap[EGizmoViewType::ZRotate] = Obj.indices;
//}
//
//void UResourceManager::LoadScaleGizmos()
//{
//    ObjData Obj = LoadObj(L"Resource/Shape/GizmoScale.obj");
//
//    TArray<FVertexSimple> XVertices;
//    TArray<FVertexSimple> YVertices;
//    TArray<FVertexSimple> ZVertices;
//
//    for (size_t i = 0; i < Obj.vertices.size(); ++i)
//    {
//        FVertexSimple xVert = Obj.vertices[i];
//        FVertexSimple yVert;
//        yVert.X = Obj.vertices[i].Y; yVert.Y = Obj.vertices[i].X; yVert.Z = Obj.vertices[i].Z; //Y축 방향
//        FVertexSimple zVert;
//        zVert.X = Obj.vertices[i].Z; zVert.Y = Obj.vertices[i].Y; zVert.Z = Obj.vertices[i].X; //Z축 방향
//
//        // X축은 빨간색 (1,0,0)
//        xVert.R = 1.f; xVert.G = 0.f; xVert.B = 0.f;
//        // Y축은 녹색 (0,1,0)
//        yVert.R = 0.f; yVert.G = 1.f; yVert.B = 0.f;
//        // Z축은 파란색 (0,0,1)
//        zVert.R = 0.f; zVert.G = 0.f; zVert.B = 1.f;
//
//        XVertices.push_back(xVert);
//        YVertices.push_back(yVert);
//        ZVertices.push_back(zVert);
//    }
//
//    GizmoVertexDataMap[EGizmoViewType::XScale] = XVertices;
//    GizmoVertexDataMap[EGizmoViewType::YScale] = YVertices;
//    GizmoVertexDataMap[EGizmoViewType::ZScale] = ZVertices;
//    GizmoIndexDataMap[EGizmoViewType::XScale] = Obj.indices;
//    GizmoIndexDataMap[EGizmoViewType::YScale] = Obj.indices;
//    GizmoIndexDataMap[EGizmoViewType::ZScale] = Obj.indices;
//}

void UResourceManager::Release()
{
}
//
//const TArray<uint32> UResourceManager::GetGizmoIndexData(EGizmoViewType Type) const
//{
//    if (GizmoIndexDataMap.contains(Type))
//    {
//        return GizmoIndexDataMap.find(Type)->second;
//    }
//    return TArray<uint32>();
//}
//
//struct DFSItem {
//    USceneComponent* Component;
//    json::JSON* ParentChildrenArray;
//};
//
//void UResourceManager::SaveScene(std::string SceneName)
//{
//    UWorld* World = UEngine::GetEngine().GetWorld();
//    if (!World)
//        return;
//
//    json::JSON Scene;
//
//    TArray<AActor*> Actors = World->GetActors();
//    TArray<USceneComponent*> RealRootComponents;
//
//    for (AActor* Element : Actors)
//        RealRootComponents.push_back(Element->GetRootComponent());
//
//    uint32 key = 0;
//    std::string ID;
//
//    for (USceneComponent* RootComponent : RealRootComponents)
//    {
//        ID = std::to_string(key);
//
//        json::JSON rootJson = json::Object();
//        rootJson["Location"] = json::FVectorToJSON(RootComponent->GetComponentLocation());
//        rootJson["Rotation"] = json::FRotatorToJSON(RootComponent->GetComponentRotation());
//        rootJson["Scale"] = json::FVectorToJSON(RootComponent->GetComponentScale());
//        rootJson["Type"] = typeid(*RootComponent).name();
//        rootJson["Children"] = json::Array();
//
//        Scene["Actors"][ID] = rootJson;
//
//        TArray<DFSItem> stack;
//        for (USceneComponent* Child : RootComponent->GetAllChildren())
//        {
//            DFSItem item;
//            item.Component = Child;
//            item.ParentChildrenArray = &Scene["Actors"][ID]["Children"];
//            stack.push_back(item);
//        }
//
//        while (!stack.empty())
//        {
//            DFSItem currentItem = stack.back();
//            stack.pop_back();
//            USceneComponent* ChildComponent = currentItem.Component;
//
//            json::JSON childJson = json::Object();
//            childJson["Location"] = json::FVectorToJSON(ChildComponent->GetComponentLocation());
//            childJson["Rotation"] = json::FRotatorToJSON(ChildComponent->GetComponentRotation());
//            childJson["Scale"] = json::FVectorToJSON(ChildComponent->GetComponentScale());
//            childJson["Type"] = typeid(*ChildComponent).name();
//            childJson["Children"] = json::Array();
//
//            currentItem.ParentChildrenArray->append(childJson);
//            int lastIndex = currentItem.ParentChildrenArray->length() - 1;
//            json::JSON& newChildJSON = (*currentItem.ParentChildrenArray)[lastIndex];
//            for (USceneComponent* GrandChild : ChildComponent->GetAllChildren())
//            {
//                if (GrandChild != nullptr)
//                {
//                    DFSItem newItem;
//                    newItem.Component = GrandChild;
//                    newItem.ParentChildrenArray = &newChildJSON["Children"];
//                    stack.push_back(newItem);
//                }
//            }
//        }
//        key++;
//    }
//    std::string jsonData = Scene.dump();
//    std::ofstream outFile("Resource/Scenes/" + SceneName + ".Scene");
//
//    std::string str = ("Resource/Scenes/" + SceneName + ".Scene");
//    wchar_t wbuf[1024];
//    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size() + 1, wbuf, ARRAYSIZE(wbuf));
//    FString wSceneName = FString(wbuf);
//
//    if (outFile.is_open())
//    {
//        outFile << jsonData;
//        outFile.close();
//        UE_LOG(LogTemp, Warning, TEXT("Success to Save %s"), wSceneName.c_str());
//    } else {
//        UE_LOG(LogTemp, Warning, TEXT("Can't access %s"), wSceneName.c_str());
//    }
//}

void UResourceManager::SetConfigData(EConfigData type, float data) {
    wchar_t ret[100] = {};
    FString attribute;
    switch(type) {
    case EConfigData::GridScale:
        attribute = FString(L"GridScale"); break;
    case EConfigData::MouseSensitive:
        attribute = FString(L"MouseSensitive"); break;
    case EConfigData::MoveSpeed:
        attribute = FString(L"MouseMoveSpeed"); break;
    }

    int res = WritePrivateProfileString(TEXT("Config"), attribute.c_str(), std::to_wstring(data).c_str(), L"./editor.ini");
    if ( !res )
        DWORD err = GetLastError();
}
float UResourceManager::GetConfigData(EConfigData type, float defaultValue = 1.0f) {
    wchar_t ret[100] = {};
    FString attribute;
    switch ( type ) {
    case EConfigData::GridScale:
        attribute = FString(L"GridScale"); break;
    case EConfigData::MouseSensitive:
        attribute = FString(L"MouseSensitive"); break;
    case EConfigData::MoveSpeed:
        attribute = FString(L"MouseMoveSpeed"); break;
    }

    int res = GetPrivateProfileString(TEXT("Config"), attribute.c_str(), L"", ret, sizeof(ret), L"./editor.ini");
    if ( !res )
        DWORD err = GetLastError();
    if ( lstrcmp(L"\0", ret) )
        return std::stof(ret);
    else
        return defaultValue;
}