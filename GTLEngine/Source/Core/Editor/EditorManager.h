#pragma once

class FViewport;
class FViewportClient;
class AActor;
class FGizmoManager;
class FSplitterH;
class FSplitterV;

class FEditorManager
{
public:
	void Init(const FWindowInfo& InWindowInfo);
	void Tick(float TickTime);
	void Draw();
	void Destroy();


	AActor* GetSelectedActor() const { return SelectedActor; }
	void SetSelectedActor(AActor* InActor) { SelectedActor = InActor; }

private:
	void UpdateHoveredViewport();
	void UpdateSelectedViewport();
	void UpdateSplitterDragging();
	//void CastRay(FRay Ray);

	//FViewportClient* ViewportClient;

private:
	TArray<FViewport> Viewports;
	FViewport* HoveredViewport;
	FViewport* SelectedViewport;

	FGizmoManager* GizmoManager;

	AActor* SelectedActor;


	FSplitterH* SplitterH;
	FSplitterV* SplitterV;
};