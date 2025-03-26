#pragma once

#include "Math/Vector.h"
#include "UI/UIBase.h"


class UControlPanel: public UUIBase
{
	DECLARE_CLASS(UControlPanel, UUIBase)
public:
	UControlPanel();

public:
	virtual void Tick(float DeltaTime) override;
	void EnvironmentPanel();
	void GizmoPanel();
	void SceneManagementPanel();
	void MemoryPanel();
	void ViewportCameraPanel();
	virtual void Destroy() override;

private:
	int32 CurrentPrimitiveType;
	uint32 SpawnNum;
	std::string SceneName;

	bool* blsOrthogonal;

	float* FOV;
	FVector* CameraLocation;
	FVector* CameraRotation;

	float WindowWidth;
	float WindowHeight;

	ImGuiIO& io = ImGui::GetIO();
};
