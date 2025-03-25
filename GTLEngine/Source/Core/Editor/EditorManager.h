#pragma once

class FViewport;
class FViewportClient;
class AActor;
class FGizmoManager;

class FEditorManager
{
public:
	void Init(const FWindowInfo& InWindowInfo);
	void Tick(float TickTime);
	void Draw();
	void Destroy();

private:
	void UpdateHoveredViewport();
	void UpdateSelectedViewport();
	//void CastRay(FRay Ray);

	//FViewportClient* ViewportClient;

private:
	TArray<FViewport> Viewports;
	FViewport* HoveredViewport;
	FViewport* SelectedViewport;

	FGizmoManager* GizmoManager;

	AActor* SelectedActor;
};




