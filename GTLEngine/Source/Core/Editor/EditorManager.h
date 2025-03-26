#pragma once

class FViewport;
class FViewportClient;
class AActor;
class FGizmoManager;
class FSplitterH;
class FSplitterV;

class UDirectXHandle;
struct FVector2;

class FEditorManager
{
public:
	void Init(const FWindowInfo& InWindowInfo, UDirectXHandle* Handle);
	void Tick(float TickTime);
	void Draw(UDirectXHandle* Handle);
	void Destroy();


	AActor* GetSelectedActor() const { return SelectedActor; }
	void SetSelectedActor(AActor* InActor) { SelectedActor = InActor; }
	TArray<FViewport>& GetViewports() { return Viewports; }
	FViewport* GetSelectedViewport() { return SelectedViewport; }

	FVector2 GetSplitterPosition() const;
	void SetSplitterPosition(FVector2 Position);

	FVector2 GetSplitterRatio() const;

	void SetSingleViewport(bool bSingleViewport);
	bool IsSingleViewport() const { return bIsSingleViewport; }

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

	bool bIsSingleViewport;
};