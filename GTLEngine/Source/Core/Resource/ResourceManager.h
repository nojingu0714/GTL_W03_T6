#pragma once

#include "Resource/Types.h"

enum class EConfigData {
    GridScale,
    MouseSensitive,
    MoveSpeed,
};

class UResourceManager
{
public:
    UResourceManager();
    ~UResourceManager();

    /*void LoadTranslateGizmos();
    void LoadRotateGizmos();
    void LoadScaleGizmos();*/

    void Release();

    //const TArray<FVertexPNCT> GetGizmoVertexData(EGizmoViewType Type) const;
    //const TArray<uint32> GetGizmoIndexData(EGizmoViewType Type) const;

    void SetConfigData(EConfigData type, float data);
    float GetConfigData(EConfigData type, float defaultValue);

private:
    TMap<EGizmoViewType, TArray<FVertexPNCT>> GizmoVertexDataMap;
    TMap<EGizmoViewType, TArray<uint32>> GizmoIndexDataMap;

private:
    UResourceManager(const UResourceManager& InResourceManager) = delete;  
    UResourceManager(UResourceManager&& InResourceManager) = delete;
    UResourceManager& operator=(const UResourceManager& InResourceManager) = delete;
    UResourceManager& operator=(UResourceManager&& InResourceManager) = delete;
};

