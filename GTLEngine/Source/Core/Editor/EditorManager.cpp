#include "pch.h"
#include "EditorManager.h"
#include "Window/Viewport.h"
#include "Window/ViewportClient.h"
#include "DirectX11/DirectXHandle.h"
#include "Engine/Engine.h"
#include "Gizmo/GizmoManager.h"
#include "Input/InputManager.h"
#include "World.h"
#include "Window/Splitter.h"


void FEditorManager::Init(const FWindowInfo& InWindowInfo, UDirectXHandle* Handle)
{
	/*const int num = 2;
	const int W = InWindowInfo.Width / num;
	const int H = InWindowInfo.Height / num;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			wchar_t buf[300];
			_itow_s(i * num + j, buf, 10);
			FViewport Viewport;
			Viewport.Init(buf, InWindowInfo.WindowHandle, W * i, H * j, W, H);
			Viewports.push_back(Viewport);
		}
	}*/
	//FViewport DefaultViewport;
	//DefaultViewport.Init(TEXT("Default_0"), InWindowInfo.WindowHandle, InWindowInfo.Width/4, InWindowInfo.Height/4, InWindowInfo.Width / 4, InWindowInfo.Height / 4);
	//Viewports.push_back(DefaultViewport);

	FViewport DefaultViewport;
	DefaultViewport.Init(Handle, TEXT("Default_0"), InWindowInfo.WindowHandle, 0, 0, InWindowInfo.Width / 2, InWindowInfo.Height / 2);
	Viewports.push_back(DefaultViewport);

	DefaultViewport.Init(Handle, TEXT("Default_1"), InWindowInfo.WindowHandle, InWindowInfo.Width / 2, 0, InWindowInfo.Width / 2, InWindowInfo.Height / 2 );
	Viewports.push_back(DefaultViewport);

	DefaultViewport.Init(Handle, TEXT("Default_2"), InWindowInfo.WindowHandle, 0, InWindowInfo.Height / 2, InWindowInfo.Width / 2, InWindowInfo.Height / 2);
	Viewports.push_back(DefaultViewport);

	DefaultViewport.Init(Handle, TEXT("Default_3"), InWindowInfo.WindowHandle, InWindowInfo.Width / 2, InWindowInfo.Height / 2, InWindowInfo.Width / 2, InWindowInfo.Height / 2);
	Viewports.push_back(DefaultViewport);

	// 뷰포트 클라이언트 생성
	//ViewportClient = new FViewportClient();
	//ViewportClient->Init();
	HoveredViewport = &Viewports[0];
	SelectedViewport = &Viewports[0];

	//Splitter 생성 
	SplitterH = new FSplitterH();
	SplitterH->Init(&Viewports[0], &Viewports[1], &Viewports[2], &Viewports[3]);
	

	SplitterV = new FSplitterV();
	SplitterV->Init(&Viewports[0], &Viewports[1], &Viewports[2], &Viewports[3]);

	GizmoManager = new FGizmoManager();
	GizmoManager->Init();

}

void FEditorManager::Tick(float DeltaTime)
{
	UpdateHoveredViewport();
	UpdateSelectedViewport();
	
	UpdateSplitterDragging();

	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();
	if (InputManager->GetMouseButton(UInputManager::EMouseButton::RIGHT))
	{
		InputManager->LockMouse();
		HoveredViewport->ProcessCameraMovement(DeltaTime);
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

void FEditorManager::Draw(UDirectXHandle* Handle)
{
	// TODO : DXDHANDLE에서 하도록 옮기기.
	UWorld* World = UEngine::GetEngine().GetWorld();

	//FGizmoManager* GizmoManager = UEngine::GetEngine().GetGizmoManager();

	// viewport (Texture2D) 에 그리기.
	for (FViewport& Viewport : Viewports)
	{
		Handle->PrepareViewport(Viewport);
		Handle->UpdateCameraMatrix(Viewport.GetCamera());

		Handle->SetLineMode();
		if (Viewport.GetCamera()->ProjectionMode == EProjectionMode::Perspective)
		{
			Handle->RenderWorldPlane(Viewport.GetCamera());
			Handle->RenderWorldXYZAxis();
		}
		Handle->RenderBoundingBox(World->GetActors());

		if (GetFlag(Viewport.ShowFlags, EEngineShowFlags::SF_Line))
			Handle->RenderDebugRays(FViewport::DebugRays);

		Handle->SetFaceMode();
		Handle->RenderObject(World->GetActors());
		Handle->RenderGizmo(GizmoManager->GetGizmoActor());
		Handle->EndRenderViewport();
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

FVector2 FEditorManager::GetSplitterPosition() const
{
	return FVector2(SplitterV->GetPosition().X, SplitterH->GetPosition().Y);
}

void FEditorManager::SetSplitterPosition(FVector2 Position)
{
	SplitterH->SetSplitterPosition(Position);
	SplitterV->SetSplitterPosition(Position);
}

FVector2 FEditorManager::GetSplitterRatio() const
{
	return FVector2(SplitterV->GetSplitterRatio().X, SplitterH->GetSplitterRatio().Y);
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

void FEditorManager::UpdateSplitterDragging()
{
	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();

	int32 MouseX = InputManager->GetMouseClientX();
	int32 MouseY = InputManager->GetMouseClientY();
	FVector2 MousePosition = FVector2(MouseX, MouseY);

	if (InputManager->GetMouseButton(UInputManager::EMouseButton::LEFT))
	{
		// 드래그 되고 있지 않은 경우
		if (false == SplitterH->bIsDragging && false == SplitterV->bIsDragging)
		{
			// 드래그 시작
			if (SplitterH->IsMouseOverSplitter(MousePosition))
			{
				SplitterH->bIsDragging = true;
				SplitterH->OnDrag(MousePosition);
			}
			if (SplitterV->IsMouseOverSplitter(MousePosition))
			{
				SplitterV->bIsDragging = true;
				SplitterV->OnDrag(MousePosition);
			}
		}
		// 드래그 중인 경우
		else
		{
			if (SplitterH->bIsDragging)
			{
				SplitterH->OnDrag(MousePosition);
			}
			if (SplitterV->bIsDragging)
			{
				SplitterV->OnDrag(MousePosition);
			}
		}

	}
	// 마우스 버튼이 떼어진 경우 드래그 종료
	else
	{
		SplitterH->bIsDragging = false;
		SplitterV->bIsDragging = false;
	}
}
		