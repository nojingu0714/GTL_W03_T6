#include "pch.h"
#include <Windows.h>
#include "Engine/Engine.h"
#include "ImGui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 각종 메시지를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_DESTROY:
        // Signal that the app should quit
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        {
            /**
            * switch문 특성상 아래 두 변수의 스코프를 현재의 case로 제한하지 않으면 다른 case에서도 접근 가능하지만, 초기화 되지 않음.
            * 따라서 컴파일 에러가 발생하므로, 변수들의 스코프를 현재의 case로 제한.
            */
            int32 NewHeight = HIWORD(lParam);
            int32 NewWidth = LOWORD(lParam);
            
            HRESULT hr = UEngine::GetEngine().ResizeWindow(NewWidth, NewHeight);
            if ( FAILED(hr) )
                assert(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // 윈도우 클래스 이름
    WCHAR WindowClass[] = L"JungleWindowClass";

    // 윈도우 타이틀바에 표시될 이름
    WCHAR Title[] = L"Game Tech Lab";

    // 각종 메시지를 처리할 함수인 WndProc의 함수 포인터를 WindowClass 구조체에 넣는다.
    WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

    // 윈도우 클래스 등록
    RegisterClassW(&wndclass);

    uint32 Width = 1600;
    uint32 Height = 900;

    RECT rc = { 0, 0, LONG(Width), LONG(Height) };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    OutputDebugString((std::to_wstring(Width) + L", " + std::to_wstring(Height) + L"\n").c_str());
    // 1024 x 1024 크기에 윈도우 생성
    HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInstance, nullptr);
    
    FWindowInfo WindowInfo = { 0, hWnd, Width, Height };

    UEngine& Engine = UEngine::GetEngine();
    bool GameLoopState = Engine.InitEngine(WindowInfo);



    while (GameLoopState)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                GameLoopState = false;
                break;
            }
        }

        Engine.Tick();
        Engine.Render();
    }

    Engine.ClearEngine();


    return 0;
}
