#include "pch.h"
#include "ControlPanel.h"

#include "UI/UIIcon.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "Engine.h"

#include "Resource/Types.h"

#include "World.h"
#include "Resource/ResourceManager.h"
#include "GameFrameWork/Camera.h"
#include "Components/CameraComponent.h"

#include "GameFrameWork/Shapes/Triangle.h"
#include "GameFrameWork/Shapes/Sphere.h"
#include "GameFrameWork/Shapes/Cube.h"
#include "GameFrameWork/Shapes/Cylinder.h"
#include "GameFrameWork/Shapes/Cone.h"

UControlPanel::UControlPanel()
	: UUIBase(), CurrentPrimitiveType(0), SpawnNum(1), SceneName("NewScene"), blsOrthogonal(nullptr), Location{ 0.f, 0.f,0.f }, Rotation{ 0.f,0.f,0.f }, Scale{ 1.f,1.f,1.f },
    FOV(nullptr), CameraLocation(nullptr), CameraRotation(nullptr), WindowWidth(360.f), WindowHeight(400.f)
{

}

void UControlPanel::Tick(float DeltaTime)
{
    ImGuiIO& io = ImGui::GetIO();
    

    float scaleX = io.DisplaySize.x / 1600.0f;
    float scaleY = io.DisplaySize.y / 900.0f;
    
    ImVec2 WinSize(WindowWidth * scaleX, WindowHeight * scaleY);

    ImGui::SetNextWindowPos(ImVec2(5, 10), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(WinSize, ImGuiCond_Appearing);

    // 제목.
    ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text("HELLO GTL!!");

    // FPS 및 창 크기출력.
    ImGui::Text("FPS %.0f (%.0f ms)", io.Framerate, 1000.0f / io.Framerate);
    ImGui::Text("Window %dx%d", UEngine::GetEngine().GetWindowInfo().Width, UEngine::GetEngine().GetWindowInfo().Height);
    ImGui::Text("Mouse %d,%d", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    
    ACamera* cam = UEngine::GetEngine().GetWorld()->GetCamera();
    if (cam)
    {
        ImGui::DragFloat("Grid Scale", &cam->GridScale, 0.1f, 0.1f, 100.f, "%.1f");
        ImGui::DragFloat("Camera speed", &cam->MoveSpeed, 0.1f, 0.1f, 100.f, "%.1f");
        ImGui::DragFloat("Mouse Sensitive", &cam->MouseSensitive, 0.1f, 0.1f, 10.f, "%.1f");
    }
    else 
    {
        ImGui::Text("Can not find main camera");
    }
    
    ImGui::Separator();

	// 폰트 설정.
    ImFont* UnicodeFont = io.Fonts->Fonts[FEATHER_FONT];
    ImGui::PushFont(UnicodeFont);

    // 버튼 사이즈.
    ImVec2 ControlButtonSize = ImVec2(32, 32);
    ImVec4 ActiveColor = ImVec4(0, 0.5, 0, 0.6f);

    UWorld* World = UEngine::GetEngine().GetWorld();
    if (!World)
        return;

    ACamera* Camera = World->GetCamera();
    if (Camera)
        UCameraComponent* camera = Camera->GetCameraComponent();

    bool IsTranslateActive = UEngine::GetEngine().GizmoModeIndex == EGizmoModeIndex::GMI_GizmoTranslate;
    if(IsTranslateActive)
        ImGui::PushStyleColor(ImGuiCol_Button, ActiveColor);
    if (ImGui::Button(ICON_GIZMO_TRANSLATE, ControlButtonSize)) // 기즈모 이동버튼
    {
        UEngine::GetEngine().GizmoModeIndex = EGizmoModeIndex::GMI_GizmoTranslate;
    }
    if (IsTranslateActive)
        ImGui::PopStyleColor();

    ImGui::SameLine(0, 5.0f);

    bool IsRotateActive = UEngine::GetEngine().GizmoModeIndex == EGizmoModeIndex::GMI_GizmoRotate;
    if (IsRotateActive)
        ImGui::PushStyleColor(ImGuiCol_Button, ActiveColor);
    if (ImGui::Button(ICON_GIZMO_ROTATE, ControlButtonSize))
    {
        UEngine::GetEngine().GizmoModeIndex = EGizmoModeIndex::GMI_GizmoRotate;
    }
    if (IsRotateActive)
        ImGui::PopStyleColor();

    ImGui::SameLine(0, 5.0f);

    bool IsScaleActive = UEngine::GetEngine().GizmoModeIndex == EGizmoModeIndex::GMI_GizmoScale;
    if (IsScaleActive)
        ImGui::PushStyleColor(ImGuiCol_Button, ActiveColor);
    if (ImGui::Button(ICON_GIZMO_SCALE, ControlButtonSize))
    {
        UEngine::GetEngine().GizmoModeIndex = EGizmoModeIndex::GMI_GizmoScale;
    }
    if (IsScaleActive)
        ImGui::PopStyleColor();

    ImGui::SameLine();
    float windowContentWidth = ImGui::GetWindowContentRegionMax().x;

    float buttonsTotalWidth = ControlButtonSize.x * 2 + 5.0f;
    float posX = windowContentWidth - buttonsTotalWidth;
    if (posX < 0)
        posX = 0;

    ImGui::SetCursorPosX(posX);

    if (ImGui::Button(ICON_BUTTON_CONSOLE, ControlButtonSize)); // console 창

    ImGui::SameLine(0, 5.0f);

    if (ImGui::Button(ICON_BUTTON_STAT, ControlButtonSize)); // stat 창

    ImGui::PopFont();

    // render mode
    const char* renderModes[3] = {
        "Lit",
        "UnLit",
        "Wireframe"
    };
    int32 viewModeIndex = static_cast<int32>(UEngine::GetEngine().ViewModeIndex);
    ImGui::Combo("Render mode", &viewModeIndex, renderModes, ARRAYSIZE(renderModes));
    UEngine::GetEngine().ViewModeIndex = static_cast<EViewModeIndex>(viewModeIndex);

    // set draw targets
    if ( ImGui::CollapsingHeader("Rendering Entity") ) {
        EEngineShowFlags flags = UEngine::GetEngine().ShowFlags;

        // primitive
        bool Primitive = GetFlag(flags, EEngineShowFlags::SF_Primitives);
        ImGui::Checkbox("Primitives", &Primitive);
        if ( Primitive )
            SetFlagOn(flags, EEngineShowFlags::SF_Primitives);
        else 
            SetFlagOff(flags, EEngineShowFlags::SF_Primitives);

        // line
        bool Line = GetFlag(flags, EEngineShowFlags::SF_Line);
        ImGui::Checkbox("Line", &Line);
        if ( Line )
            SetFlagOn(flags, EEngineShowFlags::SF_Line);
        else
            SetFlagOff(flags, EEngineShowFlags::SF_Line);

        // BillboardText
        bool BillboardText = GetFlag(flags, EEngineShowFlags::SF_BillboardText);
        ImGui::Checkbox("UUID", &BillboardText);
        if ( BillboardText )
            SetFlagOn(flags, EEngineShowFlags::SF_BillboardText);
        else 
            SetFlagOff(flags, EEngineShowFlags::SF_BillboardText);

        UEngine::GetEngine().ShowFlags = flags;
    }


    ImGui::Separator();

    // Primitive Spawn 창.
    // SceneManager로 이전
    //DrawSpawnPrimitive();

    //ImGui::Separator();


	// Scene 로드 세이브.
    ImGui::PushFont(UnicodeFont);

    UResourceManager* ResourceManager = UEngine::GetEngine().GetResourceManager();
    if (!ResourceManager)
        return;

    if (ImGui::Button(ICON_BUTTON_NEW_SCENE, ControlButtonSize))     // New Scene
    {
        Camera->SaveConfig();
        ResourceManager->NewScene();
    }

    ImGui::SameLine(0, 5.0f);
    if (ImGui::Button(ICON_BUTTON_LOAD_SCENE, ControlButtonSize))   // Load Scene
    {
        ResourceManager->LoadScene(SceneName);
    }


    ImGui::SameLine(0, 5.0f);
    if (ImGui::Button(ICON_BUTTON_SAVE_SCENE, ControlButtonSize))   // Save Scene
    {
        ResourceManager->SaveScene(SceneName);
    }

    ImGui::PopFont();

    char buf[20];
    strncpy_s(buf, SceneName.c_str(), sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';

    ImGui::SetNextItemWidth(150);

    if (ImGui::InputText(".Scene", buf, sizeof(buf)))
    {
        SceneName = buf;
    }

    ImGui::Separator();

    //ImGui::Separator();

    // TODO: 메모리 가져와서 UI 업데이트 해주기.
    
    ImGui::Text("Allocation Bytes %d", FPlatformMemory::GetAllocationBytes());
    ImGui::Text("Allocation Count %d", FPlatformMemory::GetAllocationCount());

    ImGui::Separator();

    ImGui::End();
}

void UControlPanel::Destroy() {}

const char* primitives[] = { "Sphere", "Cube", "Triangle", "Cylinder", "Cone"};

void UControlPanel::DrawSpawnPrimitive()
{
	
    // 여기부터 Primitive

    ImGui::SetNextItemWidth(100);

    ImGui::Combo("Primitive", &CurrentPrimitiveType, Items, IM_ARRAYSIZE(Items));

    ImGui::SameLine(0, 5.0f);

    if (CreateCustomInputInt("Number Of Spawn", ImGuiDataType_S32, &SpawnNum, "%d", ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsDecimal))
    {
        std::cout << "SpawnNum : " << SpawnNum << std::endl;

        UWorld* World = UEngine::GetEngine().GetWorld();
        if (!World)
            return;

        // 액터 스폰.
        switch (static_cast<EPrimitiveType>(CurrentPrimitiveType))
        {
        case EPrimitiveType::Triangle:
            World->SpawnActor<ATriangle>(TEXT("Triangle"), FVector(Location[0], Location[1], Location[2]), FRotator(Rotation[0], Rotation[1], Rotation[2]), FVector(Scale[0], Scale[1], Scale[2]), nullptr);
            break;
        case EPrimitiveType::Sphere:
            World->SpawnActor<ASphere>(TEXT("Sphere"), FVector(Location[0], Location[1], Location[2]), FRotator(Rotation[0], Rotation[1], Rotation[2]), FVector(Scale[0], Scale[1], Scale[2]), nullptr);
            break;
        case EPrimitiveType::Cube:
            World->SpawnActor<ACube>(TEXT("Cube"), FVector(Location[0], Location[1], Location[2]), FRotator(Rotation[0], Rotation[1], Rotation[2]), FVector(Scale[0], Scale[1], Scale[2]), nullptr);
            break;
        case EPrimitiveType::Cylinder:
            World->SpawnActor<ACylinder>(TEXT("Cylinder"), FVector(Location[0], Location[1], Location[2]), FRotator(Rotation[0], Rotation[1], Rotation[2]), FVector(Scale[0], Scale[1], Scale[2]), nullptr);
            break;
        case EPrimitiveType::Cone:
            World->SpawnActor<ACone>(TEXT("Cone"), FVector(Location[0], Location[1], Location[2]), FRotator(Rotation[0], Rotation[1], Rotation[2]), FVector(Scale[0], Scale[1], Scale[2]), nullptr);
            break;
        default:
            break;
        }
    }

	ImGui::DragFloat3("Location", Location);
	ImGui::DragFloat3("Rotation", Rotation);
	ImGui::DragFloat3("Scale", Scale);
}


bool UControlPanel::CreateCustomInputInt(const char* label, ImGuiDataType data_type, void* p_data, const char* format, ImGuiInputTextFlags flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    IM_ASSERT((flags & ImGuiInputTextFlags_EnterReturnsTrue) == 0); // Not supported by InputScalar()!

    if (format == NULL)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

    void* p_data_default = (g.NextItemData.HasFlags & ImGuiNextItemDataFlags_HasRefVal) ? &g.NextItemData.RefVal : &g.DataTypeZeroValue;

    char buf[64];
    if ((flags & ImGuiInputTextFlags_DisplayEmptyRefVal) && ImGui::DataTypeCompare(data_type, p_data, p_data_default) == 0)
        buf[0] = 0;
    else
        ImGui::DataTypeFormatString(buf, IM_ARRAYSIZE(buf), data_type, p_data, format);

    g.NextItemData.ItemFlags |= ImGuiItemFlags_NoMarkEdited;
    flags |= ImGuiInputTextFlags_AutoSelectAll | (ImGuiInputTextFlags)ImGuiInputTextFlags_LocalizeDecimalPoint;

    bool value_changed = false;
    const float button_size = ImGui::GetFrameHeight();

    ImGui::BeginGroup();
    ImGui::PushID(label);
    ImGui::SetNextItemWidth(ImMax(1.0f, ImGui::CalcItemWidth() - (button_size + style.ItemInnerSpacing.x) * 6 + 15));
    if (ImGui::InputText("", buf, IM_ARRAYSIZE(buf), flags))
        ImGui::DataTypeApplyFromText(buf, data_type, p_data, format, (flags & ImGuiInputTextFlags_ParseEmptyRefVal) ? p_data_default : NULL);
    IMGUI_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Inputable);

    const ImVec2 backup_frame_padding = style.FramePadding;
    style.FramePadding.x = style.FramePadding.y;
    if (flags & ImGuiInputTextFlags_ReadOnly)
        ImGui::BeginDisabled();
    ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    if (ImGui::ButtonEx("Spawn", ImVec2(button_size * 4.2f, button_size)))
    {
        value_changed = true;
    }
    ImGui::PopItemFlag();
    if (flags & ImGuiInputTextFlags_ReadOnly)
        ImGui::EndDisabled();

    style.FramePadding = backup_frame_padding;

    ImGui::PopID();
    ImGui::EndGroup();


    g.LastItemData.ItemFlags &= ~ImGuiItemFlags_NoMarkEdited;
    if (value_changed)
        ImGui::MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}


//void UControlPanel::DrawSpawnPrimitive()
//{
//    ImGui::PushItemWidth(200);
//    ImGui::Combo("Primitive", &currentPrimitive, primitives, IM_ARRAYSIZE(primitives));
//    ImGui::PopItemWidth();
//    if (ImGui::Button("Spawn")) 
//    {
//        UIManager->SpawnPrimitive();
//    }
//    ImGui::SameLine();
//
//    if (spawnNum < 1) spawnNum = 1;
//    // 숫자를 문자열로 변환
//
//    ImGui::PushItemWidth(148);
//    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 0.0f);
//    ImGui::SliderInt("Number of spawn", &spawnNum, 1, 50);
//    ImGui::PopStyleVar();
//    ImGui::PopItemWidth();
//    ImGui::Separator();
//}
//
//void UControlPanel::DrawSceneManage()
//{
//    ImGui::PushItemWidth(200);
//    ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));
//    ImGui::PopItemWidth();
//    if (ImGui::Button("New Scene")) 
//    {
//        UIManager->MakeNewScene();
//    }
//    if (ImGui::Button("Save Scene")) 
//    {
//        UIManager->SaveScene(sceneName);
//    }
//    if (ImGui::Button("Load Scene")) 
//    {
//        UIManager->LoadScene(sceneName);
//    }
//    ImGui::Separator();
//}
//
//void UControlPanel::DrawCameraSetting()
//{
//    bool bOrthogonal = false;
//    ImGui::Checkbox("Orthogonal", &bOrthogonal);
//
//    float fovValue = -1.0f;
//    UIManager->GetFOV(fovValue);
//
//    ImGui::PushItemWidth(200);
//    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 0.0f);
//    if (ImGui::SliderFloat("FOV", &fovValue, 0.0f, 90.0f))
//        UIManager->OnFOVChanged(fovValue);
//    ImGui::PopStyleVar();
//    ImGui::PopItemWidth();
//
//    FVector cameraLocation;
//    UIManager->GetCameraLocation(cameraLocation);
//    ImGui::PushItemWidth(67);
//    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 0.0f);
//    bool valueChanged = false;
//    if (ImGui::SliderFloat("##cameraLocationX", &cameraLocation.X, -10.f, 10.f))
//        valueChanged = true;
//    ImGui::SameLine();
//    if (ImGui::SliderFloat("##cameraLocationY", &cameraLocation.Y, -10.f, 10.f))
//        valueChanged = true;
//    ImGui::SameLine();
//    if (ImGui::SliderFloat("##cameraLocationZ", &cameraLocation.Z, -10.f, 10.f))
//        valueChanged = true;
//
//    if (valueChanged)
//        UIManager->OnCameraLocationChanged(cameraLocation);
//    ImGui::SameLine();
//    ImGui::Text("Camera Location");
//    ImGui::PopStyleVar();
//    ImGui::PopItemWidth();
//
//    FVector cameraRotation;
//    UIManager->GetCameraRotation(cameraRotation);
//    ImGui::PushItemWidth(67);
//    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 0.0f);
//    valueChanged = false;
//    if (ImGui::SliderFloat("##cameraRotationX", &cameraRotation.X, -10.f, 10.f))
//        valueChanged = true;
//    ImGui::SameLine();
//    if (ImGui::SliderFloat("##cameraRotationY", &cameraRotation.Y, -10.f, 10.f))
//        valueChanged = true;
//    ImGui::SameLine();
//    if (ImGui::SliderFloat("##cameraRotationZ", &cameraRotation.Z, -10.f, 10.f))
//        valueChanged = true;
//    
//    if (valueChanged)
//        UIManager->OnCameraRotationChanged(cameraRotation);
//
//    ImGui::SameLine();
//    ImGui::Text("Camera Rotation");
//    ImGui::PopStyleVar();
//    ImGui::PopItemWidth();
//
//    ImGui::Separator();
//    ImGui::Spacing();
//    ImGui::Text("Select Gizmo Mode");
//    if (ImGui::Button("Translation", ImVec2(105, 25))) 
//    {
//        UIManager->OnGizmoModeChanged(0);
//    }
//    ImGui::SameLine();
//    if (ImGui::Button("Rotation", ImVec2(105, 25))) 
//    {
//        UIManager->OnGizmoModeChanged(1);
//    }
//    ImGui::SameLine();
//    if (ImGui::Button("Scale", ImVec2(105, 25))) 
//    {
//        UIManager->OnGizmoModeChanged(2);
//    }
//
//}
