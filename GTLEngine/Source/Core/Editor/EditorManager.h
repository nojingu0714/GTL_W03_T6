#pragma once

class FViewport;
class FViewportClient;
class FSplitterH;
class FSplitterV;

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
	//FViewportClient* ViewportClient;

public:
	TArray<FViewport> Viewports;
	FViewport* HoveredViewport;
	FViewport* SelectedViewport;

	FSplitterH* SplitterH;
	FSplitterV* SplitterV;
};