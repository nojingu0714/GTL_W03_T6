#include "pch.h"
#include "EditorManager.h"
#include "Window/Viewport.h"
#include "Window/ViewportClient.h"
#include "DirectX11/DirectXHandle.h"
#include "Engine/Engine.h"
#include "Gizmo/GizmoManager.h"
#include "Input/InputManager.h"
#include "Window/Splitter.h"

void FEditorManager::Init(const FWindowInfo& InWindowInfo)
{

	FViewport DefaultViewport;
	DefaultViewport.Init(TEXT("Default_0"), InWindowInfo.WindowHandle, 0, 0, InWindowInfo.Width / 2 - 5, InWindowInfo.Height / 2 - 5);
	Viewports.push_back(DefaultViewport);

	DefaultViewport.Init(TEXT("Default_1"), InWindowInfo.WindowHandle, 0, InWindowInfo.Height / 2 + 5, InWindowInfo.Width / 2 - 5, InWindowInfo.Height / 2 - 5);
	Viewports.push_back(DefaultViewport);

	DefaultViewport.Init(TEXT("Default_2"), InWindowInfo.WindowHandle, InWindowInfo.Width / 2 + 5, 0, InWindowInfo.Width / 2, InWindowInfo.Height / 2 - 5);
	Viewports.push_back(DefaultViewport);

	DefaultViewport.Init(TEXT("Default_3"), InWindowInfo.WindowHandle, InWindowInfo.Width / 2 + 5, InWindowInfo.Height / 2  + 5, InWindowInfo.Width / 2, InWindowInfo.Height / 2 - 5);
	Viewports.push_back(DefaultViewport);

	// 뷰포트 클라이언트 생성
	//ViewportClient = new FViewportClient();
	//ViewportClient->Init();
	HoveredViewport = &Viewports[0];
	SelectedViewport = &Viewports[0];

	//Splitter 생성 
	SplitterH = new FSplitterH();
	SplitterH->Init(&Viewports[0], &Viewports[2], &Viewports[1], &Viewports[3]);
	

	SplitterV = new FSplitterV();
	SplitterV->Init(&Viewports[0], &Viewports[2], &Viewports[1], &Viewports[3]);
}

void FEditorManager::Tick(float DeltaTime)
{
	UpdateHoveredViewport();
	UpdateSelectedViewport();

	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();

	if (InputManager->GetMouseButton(UInputManager::EMouseButton::LEFT))
	{
		int32 MouseX = InputManager->GetMouseClientX();
		int32 MouseY = InputManager->GetMouseClientY();
		FVector2 MousePosition = FVector2(MouseX, MouseY);
		if (SplitterH->IsMouseOverSplitter(MousePosition))
		{
			SplitterH->OnDrag(MousePosition);
		}
		if (SplitterV->IsMouseOverSplitter(MousePosition))
		{
			SplitterV->OnDrag(MousePosition);
		}
	}


	if (InputManager->GetMouseButton(UInputManager::EMouseButton::RIGHT))
	{
		InputManager->LockMouse();
		HoveredViewport->ProcessInput(DeltaTime);
	}
	else
	{
		InputManager->UnLockMouse();
	}

	SelectedViewport->TickWhenSelected(DeltaTime);
	HoveredViewport->TickWhenHovered(DeltaTime);

	for (FViewport& Viewport : Viewports)
	{
		Viewport.Tick(DeltaTime);
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

void FEditorManager::UpdateHoveredViewport()
{
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();

	int x = InputManager->GetMouseClientX();
	int y = InputManager->GetMouseClientY();

	for (FViewport& Viewport : Viewports)
	{
		if (Viewport.Contains(x, y))
		{
			HoveredViewport = &Viewport;
			//UE_LOG(LogTemp, Display, TEXT("Hovered Viewprot : %s"), HoveredViewport->GetName().c_str());
			return;
		}
	}
	// 마우스 밖으로 나가면 원래의 뷰포트 선택하도록 의도.
}

void FEditorManager::UpdateSelectedViewport()
{
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();

	if (InputManager->GetMouseDown(UInputManager::EMouseButton::LEFT))
	{
		int x = InputManager->GetMouseClientX();
		int y = InputManager->GetMouseClientY();
		for (FViewport& Viewport : Viewports)
		{
			if (Viewport.Contains(x, y))
			{
				SelectedViewport = &Viewport;
				UE_LOG(LogTemp, Display, TEXT("Selected Viewport : %s"), SelectedViewport->GetName().c_str());
				return;
			}
		}
	}
}

