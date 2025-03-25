
#include "pch.h"
#include "InputManager.h"

#include "Engine.h"
#include "Core/UI/UIManager.h"

UInputManager::UInputManager()
    : CurrentKeyStates(TArray<bool>(256, false))
    , PrevKeyStates(TArray<bool>(256, false))
{
    ImGuiManager = UEngine::GetEngine().GetUIManager();
}

UInputManager::~UInputManager()
{
    CurrentKeyStates.clear();
    PrevKeyStates.clear();
}

void UInputManager::Tick(float TickTime)
{
    // 현재 키 상태를 이전 상태에 복사
    PrevKeyStates = CurrentKeyStates;
    // 0~255 범위의 가상키에 대해 GetAsyncKeyState를 호출하여 현재 상태 업데이트
    for (int i = 0; i < 256; ++i)
    {
        CurrentKeyStates[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    // 마우스 버튼 상태 업데이트
    CurrentMouseState.LeftButton = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    CurrentMouseState.RightButton = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    CurrentMouseState.MiddleButton = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

    // 마우스 커서 위치 업데이트
    FWindowInfo WindowInfo = UEngine::GetEngine().GetWindowInfo();
    POINT pt;

    if (!IsMouseLocked)
    {
        // 현재 마우스 상태를 이전 상태에 복사
        PrevMouseState = CurrentMouseState;
        if (GetCursorPos(&pt))
        {
            CurrentMouseState.ScreenX = pt.x;
            CurrentMouseState.ScreenY = pt.y;
            if (ScreenToClient(WindowInfo.WindowHandle, &pt))
            {
                CurrentMouseState.ClientX = pt.x;
                CurrentMouseState.ClientY = pt.y;
                ConvertMouseToNDC(WindowInfo.WindowHandle, WindowInfo.Width, WindowInfo.Height);
            
            }
        }
    }
    // Lock 되어있을때에는 다른 방법으로 복사
    else
    {
        if (GetCursorPos(&pt))
        {
            PrevMouseState.ScreenX = 2 * MousePosWhenLocked.x - pt.x;
            PrevMouseState.ScreenY = 2 * MousePosWhenLocked.y - pt.y;
            pt = { PrevMouseState.ScreenX,PrevMouseState.ScreenY };
            ScreenToClient(WindowInfo.WindowHandle, &pt);
            PrevMouseState.ClientX = pt.x;
            PrevMouseState.ClientY = pt.y;

            CurrentMouseState.ScreenX = MousePosWhenLocked.x;
            CurrentMouseState.ScreenY = MousePosWhenLocked.y;
            pt = { MousePosWhenLocked.x,MousePosWhenLocked.y };
            ScreenToClient(WindowInfo.WindowHandle, &pt);
            CurrentMouseState.ClientX = pt.x;
            CurrentMouseState.ClientY = pt.y;

            SetCursorPos(MousePosWhenLocked.x, MousePosWhenLocked.y);
        }
        SetCursorPos(MousePosWhenLocked.x, MousePosWhenLocked.y);

    }
}

void UInputManager::Destroy()
{
}

bool UInputManager::GetKey(int key) const
{
    if ( ImGuiManager->IsImGuiWantTextInput() )
        return false;
    return CurrentKeyStates[key];
}

bool UInputManager::GetKeyDown(int key) const
{
    if ( ImGuiManager->IsImGuiWantTextInput() )
        return false;
    return CurrentKeyStates[key] && !PrevKeyStates[key];
}

bool UInputManager::GetKeyUp(int key) const
{
    if ( ImGuiManager->IsImGuiWantTextInput() )
        return false;
    return !CurrentKeyStates[key] && PrevKeyStates[key];
}

void UInputManager::LockMouse()
{
    if (IsMouseLocked) return;
    GetCursorPos(&MousePosWhenLocked);
    ShowCursor(FALSE);
    IsMouseLocked = true;
}

void UInputManager::UnLockMouse()
{
    if (!IsMouseLocked) return;
    ShowCursor(TRUE);
    IsMouseLocked = false;
}

void UInputManager::ConvertMouseToNDC(HWND hWnd, int Width, int Height)
{
    // 마우스 커서 위치를 NCD 좌표로 변환
    float HalfWidth = Width / 2.f;
    float HalfHeight = Height / 2.f;
    CurrentMouseState.NdcX = (CurrentMouseState.ClientX - HalfWidth) / HalfWidth;
    CurrentMouseState.NdcY = (CurrentMouseState.ClientY - HalfHeight) / HalfHeight * -1.f;
}

bool UInputManager::GetMouseButton(EMouseButton button) const
{
    if ( ImGuiManager->IsImGuiWantMouseInput() )
        return false;
    if (button == EMouseButton::LEFT)
        return CurrentMouseState.LeftButton;
    if (button == EMouseButton::RIGHT)
        return CurrentMouseState.RightButton;
    if (button == EMouseButton::MIDDLE)
        return CurrentMouseState.MiddleButton;
    return false;
}

bool UInputManager::GetMouseDown(EMouseButton button) const
{
    if ( ImGuiManager->IsImGuiWantMouseInput() )
        return false;
    if (button == EMouseButton::LEFT)
        return CurrentMouseState.LeftButton && !PrevMouseState.LeftButton;
    if (button == EMouseButton::RIGHT)
        return CurrentMouseState.RightButton && !PrevMouseState.RightButton;
    if (button == EMouseButton::MIDDLE)
        return CurrentMouseState.MiddleButton && !PrevMouseState.MiddleButton;
    return false;
}

bool UInputManager::GetMouseUp(EMouseButton button) const
{
    if ( ImGuiManager->IsImGuiWantMouseInput() )
        return false;
    if (button == EMouseButton::LEFT)
        return !CurrentMouseState.LeftButton && PrevMouseState.LeftButton;
    if (button == EMouseButton::RIGHT)
        return !CurrentMouseState.RightButton && PrevMouseState.RightButton;
    if (button == EMouseButton::MIDDLE)
        return !CurrentMouseState.MiddleButton && PrevMouseState.MiddleButton;
    return false;
}
