#pragma once

#include "Object.h"
#include "Math/Vector.h"
#include "ImGui/imgui.h"
#include "CoreUObject/UI/ConsolePanel.h"

class UUIBase;
class UGizmoManager;
class UConsolePanel;
class IDragable;

class UUIManager
{

public:
	void InitUI(const FWindowInfo& WindowInfo, ID3D11Device* DXDDevice, ID3D11DeviceContext* DXDDeviceContext);

	void Tick(float DeltaTime);
	void Destroy();

	void RegistUI(UUIBase* NewUI);
	void CreateDefaultUI();
	void RenderUI();
	void UnRegistUI();

private:
	TArray<UUIBase*> UIList;
	UConsolePanel* Console;
public:
	inline const bool IsImGuiWantTextInput() const { return ImGui::GetIO().WantTextInput; }
	inline const bool IsImGuiWantMouseInput() const { return ImGui::GetIO().WantCaptureMouse; }
	inline UConsolePanel* GetConsole() const { return Console; }

	// Property Window Function
	bool GetObjectTranslation(FVector& outTranslation);
	void OnObjectTranslationChanged(FVector& inTranslation);

	bool GetObjectRotation(FVector& outRotation);
	void OnObjectRotationChanged(FVector& inRotation);

	bool GetObjectScale(FVector& outScale);

	bool GetFOV(float& outFOV);
	void OnFOVChanged(float& inFOV);

	bool GetCameraLocation(FVector& outLocation);
	void OnCameraLocationChanged(FVector& inLocation);

	bool GetCameraRotation(FVector& outRotation);
	void OnCameraRotationChanged(FVector& inRotation);

	void OnGizmoModeChanged(int modeNum);

	void CreateUsingFont();

	void PreferenceStyle();
public:
	// Console Function

};