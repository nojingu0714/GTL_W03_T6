#include "pch.h"
#include "Engine.h"
#include <stdarg.h>

#include "DirectX11/DirectXHandle.h"
#include "Time/TimeManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Editor/EditorManager.h"
#include "Asset/AssetManager.h"
#include "Asset/SceneManager.h"

#include "World.h"
#include "CoreUObject/GameFrameWork/Camera.h"
#include "GameFrameWork/Actor.h"

#include "Core/Window/Viewport.h"
#include "Core/Window/ViewportClient.h"

#include <stdlib.h>

bool UEngine::InitEngine(const FWindowInfo& InWindowInfo)
{
    WindowInfo = InWindowInfo;

    FAssetManager::Get().InitAssetManager();

    DirectX11Handle = new UDirectXHandle();
    HRESULT hr = DirectX11Handle->CreateDirectX11Handle(WindowInfo.WindowHandle);
    if (FAILED(hr))
    {
        MessageBox(WindowInfo.WindowHandle, TEXT("DirectX11 핸들 생성 실패"), TEXT("Error"), MB_OK);
        return false;
    }
    
    // 기본 뷰포트 설정.
    DirectX11Handle->InitWindow(InWindowInfo.WindowHandle, InWindowInfo.Width, InWindowInfo.Height);

    //const int num = 3;
    //const int W = InWindowInfo.Width / num;
    //const int H = InWindowInfo.Height / num;
    //for (int i = 0; i < num; i++)
    //{
    //    for (int j = 0; j < num; j++)
    //    {
    //        wchar_t buf[300];
    //        _itow_s(i * num + j, buf, 10);
    //        FViewport Viewport;
    //        Viewport.Init(buf, InWindowInfo.WindowHandle, W * i, H * j, W, H);
    //        Viewports.push_back(Viewport);
    //    }
    //}
	
    // 텍스쳐용 UV 버퍼 추가.

    // for batch line rendering
    hr = DirectX11Handle->CheckAndAddDynamicVertexBuffer<FVertexPNCT>(L"Dynamic", 1024);
    if ( FAILED(hr) ) {
        MessageBox(WindowInfo.WindowHandle, TEXT("버텍스 버퍼 생성 실패"), TEXT("Error"), MB_OK);
        return false;
    }

	// TimeManager 추가
	TimeManager = new UTimeManager();
	TimeManager->Initialize();

    // UI 매니저 추가.
	UIManager = new UUIManager();
	UIManager->InitUI(WindowInfo, DirectX11Handle->GetD3DDevice(), DirectX11Handle->GetD3DDeviceContext());

    // 인풋 매니저 추가.
    InputManager = new UInputManager();

    // 에디터 매니저 추가.
	EditorManager = new FEditorManager();
	EditorManager->Init(InWindowInfo, DirectX11Handle);

    // 월드 추가.
    //ResourceManager->LoadScene("DefaultScene");

	FSceneManager::LoadScene("DefaultScene");

    return true;
}

void UEngine::Tick()
{
    TickWindowInfo();

    // TimeManager.
    TimeManager->Update();

	// InputManager.
    InputManager->Tick(TimeManager->DeltaTime());

    // UIManager
    UIManager->Tick(TimeManager->DeltaTime());

    // World 오브젝트 값들 없데이트.
    World->Tick(TimeManager->DeltaTime());

    // EditorManager Tick
	EditorManager->Tick(TimeManager->DeltaTime());
}

void UEngine::TickWindowInfo() {
    RECT rect;
    if ( GetClientRect(WindowInfo.WindowHandle, &rect) ) {
        WindowInfo.Left = rect.left;
        WindowInfo.Right = rect.right;
        WindowInfo.Top = rect.top;
        WindowInfo.Bottom = rect.bottom;
        WindowInfo.Width = rect.right - rect.left;
        WindowInfo.Height = rect.bottom - rect.top;    
    }
}

void UEngine::Render()
{

    EditorManager->Draw(DirectX11Handle);
    

    UIManager->RenderUI();
    DirectX11Handle->RenderWindow();

}

HRESULT UEngine::ResizeWindow(int width, int height)
{

    WindowInfo.Width = width;
    WindowInfo.Height = height;

    // Init 되기 전에도 실행됨
    HRESULT hr = S_OK;
    if (DirectX11Handle) {
        hr = DirectX11Handle->ResizeWindow(width, height);
        DirectX11Handle->ResizeViewport(width, height);

        // Splitter 비율 가져오기 (예: 0.5, 0.3 등등)
        FVector2 SplitterRatio = EditorManager->GetSplitterRatio();
        // 새로운 픽셀 위치로 변환
		FVector2 NewSplitterPos = FVector2(width * SplitterRatio.X, height * SplitterRatio.Y);

		auto& Viewports = EditorManager->GetViewports();
        for (int32 i = 0; i < Viewports.size(); ++i)
        {
			FViewport& Viewport = Viewports[i];

            bool IsTop = (i < 2); // 0, 1은 상단, 2, 3은 하단
			bool IsLeft = ((i % 2) == 0); // 0, 2는 좌측, 1, 3은 우측

            float TopLeftX = 0.f;
			float TopLeftY = 0.f;
			float Width = 0.f;
			float Height = 0.f;

            // 세로 위치 결정
            if (IsTop)
            {
                TopLeftY = 0.f;
				Height = NewSplitterPos.Y;
            }
            else
            {
                TopLeftY = NewSplitterPos.Y;
				Height = height - NewSplitterPos.Y;
            }

            // 가로 위치 결정
            if (IsLeft)
            {
				TopLeftX = 0.f;
				Width = NewSplitterPos.X;
			}
            else
            {
				TopLeftX = NewSplitterPos.X;
				Width = width - NewSplitterPos.X;
            }

   //         switch (i)
   //         {
   //         case 0: // 좌상단
			//	TopLeftX = 0;
			//	TopLeftY = 0;
			//	Width = NewSplitterPos.X;
			//	Height = NewSplitterPos.Y;
			//	break;
			//case 1: // 우상단
			//	TopLeftX = NewSplitterPos.X;
			//	TopLeftY = 0;
			//	Width = width - NewSplitterPos.X;
			//	Height = NewSplitterPos.Y;
			//	break;
			//case 2: // 좌하단
			//	TopLeftX = 0;
			//	TopLeftY = NewSplitterPos.Y;
			//	Width = NewSplitterPos.X;
			//	Height = height - NewSplitterPos.Y;
			//	break;
			//case 3: // 우하단
			//	TopLeftX = NewSplitterPos.X;   
			//	TopLeftY = NewSplitterPos.Y;
			//	Width = width - NewSplitterPos.X;
			//	Height = height - NewSplitterPos.Y;
			//	break;
   //         }

            
            Viewport.MoveViewport(static_cast<UINT>(TopLeftX), static_cast<UINT>(TopLeftY));
            Viewport.ResizeViewport(static_cast<UINT>(Width), static_cast<UINT>(Height));
        }
		EditorManager->SetSplitterPosition(NewSplitterPos);
    }
    return hr;
}

void UEngine::ClearEngine()
{
    if (DirectX11Handle)
    {
        DirectX11Handle->ReleaseDirectX11Handle();
    }

    if (World)
    {
        World->Destroy();
		FObjectFactory::DestroyObject(World);
    }

    if (UIManager)
	{
        UIManager->Destroy();
		delete UIManager;
		UIManager = nullptr;
	}

    if (InputManager)
    {
		InputManager->Destroy();
        delete InputManager;
        InputManager = nullptr;
    }

	if (TimeManager) 
	{
		delete TimeManager;
		TimeManager = nullptr;
	}

    if (EditorManager)
    {
        EditorManager->Destroy();
		delete EditorManager;
		EditorManager = nullptr;
    }
}


void UEngine::CreateNewWorld()
{
    if (World)
    {
        DestroyWorld();
    }
    World = UWorld::CreateWorld();
    //World->CreateDefaultUI();
}

void UEngine::DestroyWorld()
{
    if (World)
    {
        World->Destroy();
        //FObjectFactory::DeleteObject();
    }
}
