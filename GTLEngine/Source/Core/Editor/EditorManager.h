#pragma once

#include "Core/Resource/Types.h"

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
	FGizmoManager* GetGizmoManager() const { return GizmoManager; }

	TArray<FViewport>& GetViewports() { return Viewports; }
	FViewport* GetSelectedViewport() { return SelectedViewport; }

	FVector2 GetSplitterPosition() const;
	void SetSplitterPosition(FVector2 Position);

	FVector2 GetSplitterRatio() const;

private:
	void UpdateHoveredViewport();
	void UpdateSelectedViewport();
	void UpdateSplitterDragging();
	//void CastRay(FRay Ray);

	//FViewportClient* ViewportClient;

public:
	FRay GetRayOnWorld();

private:
	TArray<FViewport> Viewports;
	FViewport* HoveredViewport;
	FViewport* SelectedViewport;

	FGizmoManager* GizmoManager;

	AActor* SelectedActor;

	FSplitterH* SplitterH;
	FSplitterV* SplitterV;
};