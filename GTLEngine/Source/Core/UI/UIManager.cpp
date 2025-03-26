#include "pch.h"
#include "UIManager.h"

#include "Math/Matrix.h"
#include "UI/UIBase.h"
#include "UI/ControlPanel.h"
#include "UI/ConsolePanel.h"
#include "UI/PropertyPanel.h"
#include "UI/WorldOutliner.h"

#include "Asset/IconDefs.h"
#include "Asset/RawFonts.h"

#include "Engine/Engine.h"
#include "Input/InputManager.h"
#include "Core/GizmoManager/GizmoManager.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"



void UUIManager::InitUI(const FWindowInfo& WindowInfo, ID3D11Device* DXDDevice, ID3D11DeviceContext* DXDDeviceContext)
{
	// ImGui 생성.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplDX11_Init(DXDDevice, DXDDeviceContext);
	ImGui_ImplWin32_Init((void*)WindowInfo.WindowHandle);

	CreateDefaultUI();
	CreateUsingFont();
}

void UUIManager::RegistUI(UUIBase* NewUI)
{
	UIList.push_back(NewUI);

	// if it's console remember it
	UConsolePanel* downcast;
	if ( downcast = Cast<UConsolePanel>(NewUI) )
		Console = downcast;
}

void UUIManager::CreateDefaultUI() {

	RegistUI(FObjectFactory::ConstructObject<UControlPanel>());
	RegistUI(FObjectFactory::ConstructObject<UConsolePanel>());
	RegistUI(FObjectFactory::ConstructObject<UPropertyPanel>());
	RegistUI(FObjectFactory::ConstructObject<UWorldOutliner>());
}

void UUIManager::Tick(float DeltaTime)
{
	PreferenceStyle();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//for (UUIBase* UI : UIList)
	for (int i = 0; i < UIList.size(); i++)
	{
		UUIBase* UI = UIList[i];
		if (UI)
		{
 			UI->Tick(DeltaTime);
		}
	}
}

void UUIManager::RenderUI()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UUIManager::UnRegistUI()
{
	for (UUIBase* UI : UIList)
	{
		UI->Destroy();
		FObjectFactory::DestroyObject(UI);
	}
}

void UUIManager::Destroy()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	UnRegistUI();
}

void UUIManager::CreateUsingFont()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	ImFontConfig FeatherFontConfig;
	FeatherFontConfig.PixelSnapH = true;
	FeatherFontConfig.FontDataOwnedByAtlas = false;
	FeatherFontConfig.GlyphOffset = ImVec2(0, 0);
	static const ImWchar IconRanges[] = {
		ICON_MOVE,      ICON_MOVE + 1,
		ICON_ROTATE,    ICON_ROTATE + 1,
		ICON_SCALE,     ICON_SCALE + 1,
		ICON_MONITOR,   ICON_MONITOR + 1,
		ICON_BAR_GRAPH, ICON_BAR_GRAPH + 1,
		ICON_NEW,       ICON_NEW + 1,
		ICON_SAVE,      ICON_SAVE + 1,
		ICON_LOAD,      ICON_LOAD + 1,
		0 };

	io.Fonts->AddFontFromMemoryTTF(FeatherRawData, FontSizeOfFeather, 22.0f, &FeatherFontConfig, IconRanges);
}

void UUIManager::PreferenceStyle()
{
	// Window
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.9f);
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImGui::GetStyle().WindowRounding = 5.0f;

	ImGui::GetStyle().FrameRounding = 3.0f;

	// Sep
	ImGui::GetStyle().Colors[ImGuiCol_Separator] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

	// Frame
	ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.31f, 0.31f, 0.31f, 0.6f);
	ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = ImVec4(0.203f, 0.203f, 0.203f, 0.6f);
	ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0f, 0.5f, 0.0f, 0.6f);

	// Button
	ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.105f, 0.105f, 0.105f, 0.6f);
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.105f, 0.105f, 0.105f, 0.6f);
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.5f, 0.0f, 0.6f);

	ImGui::GetStyle().Colors[ImGuiCol_Header] = ImVec4(0.203f, 0.203f, 0.203f, 0.6f);
	ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = ImVec4(0.105f, 0.105f, 0.105f, 0.6f);
	ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.5f, 0.0f, 0.6f);

	// Text
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 0.9f);

}