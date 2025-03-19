#pragma once

#include "Math/Vector.h"
#include "UI/UIBase.h"


static const char* Items[] = {  "None", "Line", "Triangle", "Sphere", "Cube", "Cylinder", "Cone" };

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
	virtual void Destroy() override;

private:
	//void DrawSpawnPrimitive();

	//bool CreateCustomInputInt(const char* label, ImGuiDataType data_type, void* p_data, const char* format, ImGuiInputTextFlags flags);

private:
	int32 CurrentPrimitiveType;
	uint32 SpawnNum;
	std::string SceneName;

	bool* blsOrthogonal;

	float* FOV;
	FVector* CameraLocation;
	FVector* CameraRotation;

	float Location[3];
	float Rotation[3];
	float Scale[3];

	float WindowWidth;
	float WindowHeight;

	ImGuiIO& io = ImGui::GetIO();
};
