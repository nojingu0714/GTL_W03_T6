#include "pch.h"
#include "EditorManager.h"
#include "Window/Viewport.h"
#include "Window/ViewportClient.h"
#include "DirectX11/DirectXHandle.h"
#include "Engine/Engine.h"
#include "Gizmo/GizmoManager.h"

void FEditorManager::Init(const FWindowInfo& InWindowInfo)
{

	FViewport DefaultViewport;
	DefaultViewport.Init(TEXT("Default_0"), InWindowInfo.WindowHandle, 0, 0, InWindowInfo.Width / 2, InWindowInfo.Height / 2);
	Viewports.push_back(DefaultViewport);

	FViewport DefaultViewport_1;
	DefaultViewport_1.Init(TEXT("Default_1"), InWindowInfo.WindowHandle, InWindowInfo.Width / 2, InWindowInfo.Height / 2, InWindowInfo.Width / 2, InWindowInfo.Height/ 2);
	Viewports.push_back(DefaultViewport_1);
	DefaultViewport_1.GetCamera()->Location = FVector(2.f, 2.f, 2.f);

	// 뷰포트 클라이언트 생성
	//ViewportClient = new FViewportClient();
	//ViewportClient->Init();
}

void FEditorManager::Tick(float TickTime)
{
	for (FViewport& Viewport : Viewports)
	{
		Viewport.Tick(TickTime);
	}
}

void FEditorManager::Draw()
{
	UDirectXHandle* Handle = UEngine::GetEngine().GetDirectX11Handle();
	UWorld* World = UEngine::GetEngine().GetWorld();
	UGizmoManager* GizmoManager = UEngine::GetEngine().GetGizmoManager();

	// viewport (Texture2D) 에 그리기.
	for (const FViewport& Viewport : Viewports)
	{
		Handle->PrepareViewport(Viewport);
		Handle->UpdateCameraMatrix(Viewport.GetCamera());

		Handle->SetLineMode();
		Handle->RenderWorldPlane(Viewport.GetCamera());
		Handle->RenderBoundingBox(World->GetActors());
		Handle->RenderLines(World->GetActors());

		Handle->SetFaceMode();
		Handle->RenderObject(World->GetActors());
		Handle->RenderGizmo(GizmoManager->GetGizmo());
	}

	Handle->PrepareWindow();

	for (FViewport& Viewport : Viewports)
	{
		Handle->RenderViewport(Viewport);
	}
}

void FEditorManager::Destroy()
{
	for (FViewport& Viewport : Viewports)
	{
		Viewport.Destroy();
	}
}
