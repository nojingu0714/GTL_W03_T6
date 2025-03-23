#include "pch.h"
#include "Types.h"

FString GetPrimitiveTypeAsString(EPrimitiveType Type)
{
    switch (Type)
    {
    case EPrimitiveType::Line:
        return FString(TEXT("Line"));
    case EPrimitiveType::Triangle:
        return FString(TEXT("Triangle"));
    case EPrimitiveType::Sphere:
        return FString(TEXT("Sphere"));
    case EPrimitiveType::Cube:
        return FString(TEXT("Cube"));
    case EPrimitiveType::Cylinder:
        return FString(TEXT("Cylinder"));
    case EPrimitiveType::Cone:
        return FString(TEXT("Cone"));
    case EPrimitiveType::Grid:
        return FString(TEXT("Grid"));
    case EPrimitiveType::BoundingBox:
        return FString(TEXT("BoundingBox"));
    case EPrimitiveType::Max:
        return FString(TEXT("Max"));
    case EPrimitiveType::None:
    default:
        return FString(TEXT("None"));
    }
}

FString GetGizmoViewTypeAsString(EGizmoViewType Type)
{
    switch (Type)
    {
    case EGizmoViewType::XTranslate:
        return FString(TEXT("XTranslate"));
    case EGizmoViewType::YTranslate:
        return FString(TEXT("YTranslate"));
    case EGizmoViewType::ZTranslate:
        return FString(TEXT("ZTranslate"));
    case EGizmoViewType::XRotate:
        return FString(TEXT("XRotate"));
    case EGizmoViewType::YRotate:
        return FString(TEXT("YRotate"));
    case EGizmoViewType::ZRotate:
        return FString(TEXT("ZRotate"));
    case EGizmoViewType::XScale:
        return FString(TEXT("XScale"));
    case EGizmoViewType::YScale:
        return FString(TEXT("YScale"));
    case EGizmoViewType::ZScale:
        return FString(TEXT("ZScale"));
    case EGizmoViewType::Max:
        return FString(TEXT("Max"));
    default:
        return FString();
    }
}

FString GetDepthComparisonModeAsString(EDepthComparisonMode Mode)
{
    switch (Mode)
    {
    case EDepthComparisonMode::Less:
		return FString(TEXT("Less"));
    case EDepthComparisonMode::Always:
		return FString(TEXT("Always"));
    default:
		return FString(TEXT("None"));
    }
}

FString GetERasterizerModeAsString(ERasterizerMode Mode)
{
	switch (Mode)
	{
	case ERasterizerMode::Solid_None:
		return FString(TEXT("Solid_None"));
	case ERasterizerMode::Solid_Front:
		return FString(TEXT("Solid_Front"));
	case ERasterizerMode::Solid_Back:
		return FString(TEXT("Solid_Back"));
	case ERasterizerMode::Wireframe_None:
		return FString(TEXT("Wireframe_None"));
	case ERasterizerMode::Wireframe_Front:
		return FString(TEXT("Wireframe_Front"));
	case ERasterizerMode::Wireframe_Back:
		return FString(TEXT("Wireframe_Back"));
	default:
		return FString(TEXT("None"));
	}
}
