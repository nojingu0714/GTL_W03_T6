#include "pch.h"
#include "ControlPanel.h"

#include "UI/UIIcon.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "Engine.h"

#include "Editor/EditorManager.h"
#include "Window/Viewport.h"
#include "Utils/GTLStringLibrary.h"

UControlPanel::UControlPanel()
	: UUIBase(), CurrentPrimitiveType(0), SpawnNum(1), SceneName("NewScene"), blsOrthogonal(nullptr), 
    //Location{ 0.f, 0.f,0.f }, Rotation{ 0.f,0.f,0.f }, Scale{ 1.f,1.f,1.f },
    FOV(nullptr), CameraLocation(nullptr), CameraRotation(nullptr), WindowWidth(360.f), WindowHeight(400.f)
{
}

void UControlPanel::Tick(float DeltaTime)
{    
    float scaleX = io.DisplaySize.x / 1600.0f;
    float scaleY = io.DisplaySize.y / 900.0f;
    
    ImVec2 WinSize(WindowWidth * scaleX, WindowHeight * scaleY);

    ImGui::SetNextWindowPos(ImVec2(5, 10), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(WinSize, ImGuiCond_Appearing);

    ImGui::Begin("System Info", nullptr);

    EnvironmentPanel();
    ImGui::Separator();
    MemoryPanel();
    ImGui::Separator();
	// CameraSpeed();
    ViewportCameraPanel();
    ImGui::Separator();
    // GridScale();
    ImGui::Separator();

    ImGui::End();
}

void UControlPanel::EnvironmentPanel()
{
    // FPS 및 창 크기출력.
    ImGui::Text("FPS %.0f (%.0f ms)", io.Framerate, 1000.0f / io.Framerate);
    ImGui::Text("Window %dx%d", UEngine::GetEngine().GetWindowInfo().Width, UEngine::GetEngine().GetWindowInfo().Height);
}

void UControlPanel::GizmoPanel()
{
    // 폰트 설정.
    ImFont* UnicodeFont = io.Fonts->Fonts[FEATHER_FONT];
    ImGui::PushFont(UnicodeFont);

    // 버튼 사이즈.
    ImVec2 ControlButtonSize = ImVec2(32, 32);
    ImVec4 ActiveColor = ImVec4(0, 0.5, 0, 0.6f);

    UWorld* World = UEngine::GetEngine().GetWorld();
    if (!World)
        return;

   /* ACamera* Camera = World->GetCamera();
    if (Camera)
        UCameraComponent* camera = Camera->GetCameraComponent();*/

}

void UControlPanel::SceneManagementPanel()
{
    // Scene 로드 세이브.
    // 폰트 설정.
    ImFont* UnicodeFont = io.Fonts->Fonts[FEATHER_FONT];
    ImGui::PushFont(UnicodeFont);

    ImVec2 ControlButtonSize = ImVec2(32, 32);
    UResourceManager* ResourceManager = UEngine::GetEngine().GetResourceManager();
    if (!ResourceManager)
        return;

    if (ImGui::Button(ICON_BUTTON_NEW_SCENE, ControlButtonSize))     // New Scene
    {
        //ResourceManager->NewScene();
    }

    ImGui::SameLine(0, 5.0f);
    if (ImGui::Button(ICON_BUTTON_LOAD_SCENE, ControlButtonSize))   // Load Scene
    {
        //ResourceManager->LoadScene(SceneName);
    }


    ImGui::SameLine(0, 5.0f);
    if (ImGui::Button(ICON_BUTTON_SAVE_SCENE, ControlButtonSize))   // Save Scene
    {
        //ResourceManager->SaveScene(SceneName);
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
}

void UControlPanel::MemoryPanel()
{
    ImGui::Text("Allocation Bytes %d", FPlatformMemory::GetAllocationBytes());
    ImGui::Text("Allocation Count %d", FPlatformMemory::GetAllocationCount());
}

void UControlPanel::ViewportCameraPanel()
{
    FViewport* SelectedViewport = UEngine::GetEngine().GetEditorManager()->GetSelectedViewport();
    if (SelectedViewport)
    {
        std::string ViewportName = UGTLStringLibrary::WStringToString(SelectedViewport->GetName());
        ImGui::Text("Viewport Name : %s", ViewportName.c_str());

        FViewportCamera* ViewportCamera = SelectedViewport->GetCamera();
        if (ViewportCamera)
        {
            ImGui::DragFloat("Camera Speed", &ViewportCamera->Speed, 1.0f, 0.1f, 50.0f);
            ImGui::DragFloat("Camera FOV", &ViewportCamera->FieldOfView, 1.0f, 0.1f, 180.0f);
            ImGui::DragFloat("Camera Sensitive", &ViewportCamera->Sensitive, 1.0f, 1.0f, 100.0f);

			bool bShowDebugLine = GetFlag(SelectedViewport->ShowFlags, EEngineShowFlags::SF_Line);
            if (ImGui::Checkbox("Spawn debug line", &bShowDebugLine))
            {
				if (bShowDebugLine)
					SetFlagOn(SelectedViewport->ShowFlags, EEngineShowFlags::SF_Line);
				else
					SetFlagOff(SelectedViewport->ShowFlags, EEngineShowFlags::SF_Line);
            }

            bool bWireFrame = SelectedViewport->GetViewModeIndex() == EViewModeIndex::VMI_Wireframe;
            if (ImGui::Checkbox("WireFrame", &bWireFrame))
            {
                if (bWireFrame)
                    SelectedViewport->SetViewModeIndex(EViewModeIndex::VMI_Wireframe);
                else
                    SelectedViewport->SetViewModeIndex(EViewModeIndex::VMI_Lit);
            }

            bool bOrthogonal = ViewportCamera->ProjectionMode == EProjectionMode::Orthogonal;
            if (ImGui::Checkbox("Orthogonal", &bOrthogonal))
            {
                if (bOrthogonal)
                    ViewportCamera->ProjectionMode = EProjectionMode::Orthogonal;
                else
                    ViewportCamera->ProjectionMode = EProjectionMode::Perspective;
            }


            if (ImGui::Combo("View Position", (int*)&ViewportCamera->ViewPosition, "Top\0Bottom\0Left\0Right\0Front\0Back\0Perspective\0\0"))
            {
				ViewportCamera->ProjectionMode = EProjectionMode::Orthogonal;
				SelectedViewport->SetViewModeIndex(EViewModeIndex::VMI_Wireframe);
				switch (ViewportCamera->ViewPosition)
				{
				case EViewPosition::Top:
					ViewportCamera->Location = FVector(0.f, 0.0f, 50.0f);
					ViewportCamera->Rotation = FRotator(-89.f, 0.f, 0.f);
					break;
				case EViewPosition::Bottom:
					ViewportCamera->Location = FVector(0.f, 0.f, -50.0f);
					ViewportCamera->Rotation = FRotator(89.f, 0.f, 0.f);
					break;
				case EViewPosition::Left:
					ViewportCamera->Location = FVector(0.0f, 50.f, 0.f);
					ViewportCamera->Rotation = FRotator(0.f, -89.0f, 0.f);
					break;
				case EViewPosition::Right:
                    ViewportCamera->Location = FVector(0.0f, -50.f, 0.f);
                    ViewportCamera->Rotation = FRotator(0.f, 89.0f, 0.f);
					break;
				case EViewPosition::Front:
					ViewportCamera->Location = FVector(50.f, 0.0f, 0.f);
					ViewportCamera->Rotation = FRotator(0.f, 180.f, 0.f);
					break;
				case EViewPosition::Back:
					ViewportCamera->Location = FVector(-50.f, 0.0f, 0.f);
					ViewportCamera->Rotation = FRotator(0.f, 0.0f, 0.f);
					break;
				default:
					ViewportCamera->ProjectionMode = EProjectionMode::Perspective;
					SelectedViewport->SetViewModeIndex(EViewModeIndex::VMI_Lit);
					break;
				}
            }

        }
    }
}

void UControlPanel::Destroy() {}
