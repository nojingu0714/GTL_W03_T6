#pragma once

class FViewport;
class FViewportClient;

class FEditorManager
{
public:
	void Init(const FWindowInfo& InWindowInfo);
	void Tick(float TickTime);
	void Draw();
	void Destroy();

private:
	TArray<FViewport> Viewports;
	FViewportClient* ViewportClient;
};

