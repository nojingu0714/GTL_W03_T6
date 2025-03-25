#pragma once

#include "Object.h"
class UUIManager;
class UInputManager
{
public:
    enum class EMouseButton
    {
        LEFT = 0,
        RIGHT,
        MIDDLE,
    };

    UInputManager();
    ~UInputManager();

    // 매 프레임 호출하여 입력 상태를 업데이트
    void Tick(float TickTime);
    void Destroy();

    // 키보드 입력 함수
    bool GetKey(int key) const;
    bool GetKeyDown(int key) const;
    bool GetKeyUp(int key) const;

    // 마우스 상태 구조체
    struct MouseState
    {
        bool LeftButton = false;
        bool RightButton = false;
        bool MiddleButton = false;
        int ScreenX = 0;
        int ScreenY = 0;
        int ClientX = 0;
        int ClientY = 0;
        float NdcX = 0.f;
        float NdcY = 0.f;
    };

    // 마우스 입력 관련 함수

    // 0: Left Button, 1: Right Button, 2: Middle Button
    bool GetMouseButton(EMouseButton button) const;
    // 0: Left Button, 1: Right Button, 2: Middle Button    
    bool GetMouseDown(EMouseButton button) const;
    // 0: Left Button, 1: Right Button, 2: Middle Button    
    bool GetMouseUp(EMouseButton button) const;
    int GetMouseClientX() const { return CurrentMouseState.ClientX;};
    int GetMouseClientY() const { return CurrentMouseState.ClientY;};
    int GetMouseScreenX() const { return CurrentMouseState.ScreenX;};
    int GetMouseScreenY() const { return CurrentMouseState.ScreenY;};
    float GetMouseNdcX() const { return CurrentMouseState.NdcX;};
    float GetMouseNdcY() const { return CurrentMouseState.NdcY;};
    int GetMouseDeltaX() const { return CurrentMouseState.ClientX - PrevMouseState.ClientX; };
    int GetMouseDeltaY() const { return CurrentMouseState.ClientY - PrevMouseState.ClientY; };

    // 마우스 숨기는 용도
public:
    void LockMouse();
    void UnLockMouse();
private:
    POINT MousePosWhenLocked;
    bool IsMouseLocked = false;

protected:
    void ConvertMouseToNDC(HWND hWnd, int Width, int Height);

private:
    // 복사 및 대입 금지
    UInputManager(const UInputManager&) = delete;
    UInputManager& operator=(const UInputManager&) = delete;
    UInputManager(UInputManager&&) = delete;
    UInputManager& operator=(UInputManager&&) = delete;

    TArray<bool> CurrentKeyStates;
    TArray<bool> PrevKeyStates;

    MouseState CurrentMouseState;
    MouseState PrevMouseState;

    UUIManager* ImGuiManager;
};
