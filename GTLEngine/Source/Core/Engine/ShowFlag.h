#pragma once

#include "EnginePch.h"

enum class EViewModeIndex : uint32
{
    VMI_Lit,
    VMI_Unlit,
    VMI_Wireframe
};

enum class EEngineShowFlags : uint64
{
    SF_Primitives,
    SF_StaticMesh,
    SF_Line,
    SF_BillboardText,
};
