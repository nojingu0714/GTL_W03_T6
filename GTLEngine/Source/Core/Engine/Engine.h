#pragma once

#include "Math/Vector.h"

class UDirectXHandle;
class UResourceManager;
class UWorld;
class UTimeManager;
class UInputManager;
class UGizmoManager;
class UObject;
class UUIManager;

enum class EViewModeIndex : uint32 {
    VMI_Lit,
    VMI_Unlit,
    VMI_Wireframe
};

enum class EGizmoModeIndex : uint32 {
    GMI_GizmoTranslate,
    GMI_GizmoRotate,
    GMI_GizmoScale
};

enum class EEngineShowFlags : uint64 {
    SF_Primitives = 1 << 0,
    SF_Line = 1 << 1,
    SF_BillboardText = 1 << 2,
};

class UEngine
{

public:
    static UEngine& GetEngine()
    {
        static UEngine Engine;
        return Engine;
    }

    bool InitEngine(const FWindowInfo& InWindowInfo);
	void Tick();
    void TickWindowInfo();
    void Render();
    HRESULT ResizeWindow(int width, int height);
	void ClearEngine();
    HRESULT AddAllVertexBuffers();

private:
    UEngine() = default;
    ~UEngine() = default;
    UEngine(const UEngine&) = delete;
    UEngine& operator=(const UEngine&) = delete;
    UEngine(UEngine&&) = delete;
    UEngine& operator=(UEngine&&) = delete;

public:
    // Getter
    UDirectXHandle* GetDirectX11Handle() const { return DirectX11Handle; }
    UResourceManager* GetResourceManager() const { return ResourceManager; }
    UWorld* GetWorld() const { return World; }
	UTimeManager* GetTimeManager() const { return TimeManager; }
    UInputManager* GetInputManager() const { return InputManager; }
    UUIManager* GetUIManager() const { return UIManager; }
	UGizmoManager* GetGizmoManager() const { return GizmoManager; }

private:
	UDirectXHandle* DirectX11Handle;
	UResourceManager* ResourceManager;
    UWorld* World;
    UTimeManager* TimeManager;
    UInputManager* InputManager;
    UUIManager* UIManager;
    UGizmoManager* GizmoManager;

public:
    const FWindowInfo& GetWindowInfo() const { return WindowInfo; }

private:
    FWindowInfo WindowInfo;

public:
	float GetFPS() const { return FPS; }
	void SetFPS(float InFPS) { FPS = InFPS; }

private:
    float FPS;

public:
    EViewModeIndex ViewModeIndex;
    EGizmoModeIndex GizmoModeIndex;
    EEngineShowFlags ShowFlags = EEngineShowFlags::SF_Primitives;

public:
    void CreateNewWorld();
    void DestroyWorld();

};

extern TArray<UObject*> GUObjectArray;
