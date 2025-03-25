#pragma once

class FViewport;
class FViewportClient;
class AActor;

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

public:
	TArray<FViewport> Viewports;
	FViewport* HoveredViewport;
	FViewport* SelectedViewport;
	AActor* SelectedActor;
};




