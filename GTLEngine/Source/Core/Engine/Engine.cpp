#include "pch.h"
#include "Engine.h"
#include <stdarg.h>

#include "DirectX11/DirectXHandle.h"
#include "Time/TimeManager.h"
#include "Input/InputManager.h"
#include "Resource/ResourceManager.h"
#include "UI/UIManager.h"
#include "UI/ConsolePanel.h"

#include "World.h"
#include "Core/Engine/LogManager.h"
#include "CoreUObject/GameFrameWork/Camera.h"
#include "GameFrameWork/Actor.h"
#include "Gizmo/GizmoManager.h"

#include "Core/Window/Viewport.h"
#include "Core/Window/ViewportClient.h"


TArray<UObject*> GUObjectArray = TArray<UObject*>();

bool UEngine::InitEngine(const FWindowInfo& InWindowInfo)
{
    WindowInfo = InWindowInfo;

    // 리소스 매니저 추가.
    ResourceManager = new UResourceManager();

    DirectX11Handle = new UDirectXHandle();
    HRESULT hr = DirectX11Handle->CreateDirectX11Handle(WindowInfo.WindowHandle);
    if (FAILED(hr))
    {
        MessageBox(WindowInfo.WindowHandle, TEXT("DirectX11 핸들 생성 실패"), TEXT("Error"), MB_OK);
        return false;
    }
    
    // 기본 뷰포트 설정.
    DirectX11Handle->InitWindow(InWindowInfo.WindowHandle, InWindowInfo.Width, InWindowInfo.Height);

	FViewport DefaultViewport;
	DefaultViewport.Init(TEXT("Default"), InWindowInfo.WindowHandle, 0, 0, InWindowInfo.Width, InWindowInfo.Height);
	Viewports.push_back(DefaultViewport);

    // 뷰포트 클라이언트 생성
	ViewportClient = new FViewportClient();
    ViewportClient->Init();

    // 버텍스 버퍼 추가.
    hr = AddAllVertexBuffers();
    if (FAILED(hr))
    {
        MessageBox(WindowInfo.WindowHandle, TEXT("버텍스 버퍼 생성 실패"), TEXT("Error"), MB_OK);
        return false;
    }

    // 텍스쳐용 UV 버퍼 추가.
    hr = DirectX11Handle->AddVertexBuffer<FVertexUV>(L"FontAtlas", ResourceManager->GetUVData(), TArray<uint32>());
	if (FAILED(hr))
	{
		MessageBox(WindowInfo.WindowHandle, TEXT("버텍스 버퍼 생성 실패"), TEXT("Error"), MB_OK);
		return false;
	}

    // for batch line rendering
    hr = DirectX11Handle->CheckAndAddDynamicVertexBuffer<FVertexSimple>(L"Dynamic", 1024);
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

    GizmoManager = new UGizmoManager();

    // 인풋 매니저 추가.
    InputManager = new UInputManager();

    // 월드 추가.
    ResourceManager->LoadScene("DefaultScene");

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

	GizmoManager->Tick(TimeManager->DeltaTime());
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
    // viewport (Texture2D) 에 그리기.
    for (const FViewport& Viewport : Viewports)
    {
        DirectX11Handle->PrepareViewport(Viewport);
        DirectX11Handle->UpdateCameraMatrix(World->GetCamera());

        DirectX11Handle->SetLineMode();
        DirectX11Handle->RenderWorldPlane(World->GetCamera());
        DirectX11Handle->RenderBoundingBox(World->GetActors());
        DirectX11Handle->RenderLines(World->GetActors());

        DirectX11Handle->SetFaceMode();
        DirectX11Handle->RenderObject(World->GetActors());
        DirectX11Handle->RenderGizmo(GizmoManager->GetGizmo());
    }

        DirectX11Handle->PrepareWindow();
    for (const FViewport& Viewport : Viewports)
    {
        // Texture2D를 쓰는 Quad를 그리기
        ViewportClient->Draw(Viewport.GetName());
    }

    UIManager->RenderUI();
    DirectX11Handle->RenderWindow();



 //   // 그릴 렌더 타겟뷰 초기화.
 //   //DirectX11Handle->InitView();
 //   DirectX11Handle->PrepareViewport(&Viewports[TEXT("Default")]);
 //   DirectX11Handle->UpdateCameraMatrix(World->GetCamera());

 //   DirectX11Handle->SetLineMode();
 //   DirectX11Handle->RenderWorldPlane(World->GetCamera());
 //   DirectX11Handle->RenderBoundingBox(World->GetActors());
 //   DirectX11Handle->RenderLines(World->GetActors());

 //   DirectX11Handle->SetFaceMode();
 //   DirectX11Handle->RenderObject(World->GetActors());
 //   DirectX11Handle->RenderGizmo(GizmoManager->GetGizmo());
 //   // 오브젝트들 받아와서 DXD 핸들에 넘겨준 후 DXD 핸들에서 해당 오브젝트 값 읽어서 렌더링에 추가.

 //   // UI 그리기.
 //   UIManager->RenderUI();
	//// 최종적으로 그린 결과물을 화면에 출력.
	//DirectX11Handle->GetDXDSwapChain()->Present(1, 0);
}

HRESULT UEngine::ResizeWindow(int width, int height) {

    WindowInfo.Width = width;
    WindowInfo.Height = height;

    // Init 되기 전에도 실행됨
    HRESULT hr = S_OK;
    if (DirectX11Handle) {
        hr = DirectX11Handle->ResizeWindow(width, height);
        DirectX11Handle->ResizeViewport(width, height);
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

    if (ResourceManager)
    {
        delete ResourceManager;
        ResourceManager = nullptr;
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
}

HRESULT UEngine::AddAllVertexBuffers()
{
    HRESULT hr = S_OK;

    for (uint32 i = 0; i < static_cast<uint32>(EPrimitiveType::Max); ++i)
    {
        EPrimitiveType Type = static_cast<EPrimitiveType>(i);
        if (Type != EPrimitiveType::None)
        {
            hr = DirectX11Handle->AddVertexBuffer(GetPrimitiveTypeAsString(Type), ResourceManager->GetPrimitiveVertexData(Type), ResourceManager->GetPrimitiveIndexData(Type));
            if (FAILED(hr))
            {
                return hr;
            }
        }
    }

    for (uint32 i = 0; i < static_cast<uint32>(EGizmoViewType::Max); ++i)
    {
        EGizmoViewType Type = static_cast<EGizmoViewType>(i);
        hr = DirectX11Handle->AddVertexBuffer(GetGizmoViewTypeAsString(Type), ResourceManager->GetGizmoVertexData(Type), ResourceManager->GetGizmoIndexData(Type));
    }
    
    return S_OK;
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
