#pragma once

#include "Math/Vector.h"
#include "Gizmo/GizmoManager.h"
#include "UI/UIBase.h"

class UPropertyPanel: public UUIBase
{
	DECLARE_CLASS(UPropertyPanel, UUIBase)
public:
	UPropertyPanel();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Destroy() override;
private:
	void WorldCoordCheckbox();
	void PropertiesInspector();


private:
	float Location[3] = { 0, 0, 0 };
	float Rotation[3] = { 0, 0, 0 };
	float Scale[3] = { 0, 0, 0 };
	
	float WindowWidth;
	float WindowHeight;
	FGizmoManager* gizmoManager;
	ImGuiIO& io = ImGui::GetIO();
};
