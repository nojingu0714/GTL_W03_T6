#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"

enum class EPrimitiveType
{
    None,
    Line,
    Triangle,
    Sphere,
    Cube,
    Cylinder,
    Cone,
    Grid,
    BoundingBox,
    Max,

};

enum class EGizmoViewType
{
    XTranslate,
    YTranslate,
    ZTranslate,
    XRotate,
    YRotate,
    ZRotate,
    XScale,
    YScale,
    ZScale,
    Max,
};

FString GetPrimitiveTypeAsString(enum class EPrimitiveType Type);

FString GetGizmoViewTypeAsString(enum class EGizmoViewType Type);

// Vertex Layout
// Position, Normal, Color, Texture coord
struct FVertexPNCT
{
    FVector Position;
    FVector Normal;
    FVector4 Color;
    FVector2 UV;
};

struct FVertexFont
{
    FVector Position;
    FVector2 UV;
};

struct FVertexInfo
{
    uint NumVertices;
    ID3D11Buffer* VertexBuffer;
};

struct FIndexInfo
{
    uint32 NumIndices;
    ID3D11Buffer* IndexBuffer;
};

struct FBufferInfo
{
    FVertexInfo VertexInfo;
    FIndexInfo IndexInfo;
};

//////////
// 
// Constant Buffers
// 
//////////
enum class EConstantBufferType
{
    None,
    ChangesOnResize,
    ChangesEveryFrame,
    ChangesEveryObject,
    MVP,
    ViewportRatio,
    Max,
};

 struct alignas(16) FCbChangesOnResize
{
    FMatrix ProjectionMatrix;
};


struct alignas(16) FCbChangesEveryFrame
{
    FMatrix ViewMatrix;
};

struct alignas(16)  FCbChangesEveryObject
{
    FMatrix WorldMatrix;
    int Flag;
    int Padding[3];
};

struct alignas(16) FCbViewportRatio
{
    float x;
    float y;
    float width;
    float height;
};

struct alignas(16) FMVP
{
    FMatrix MVP;
};

struct FCbLine
{
    FVector Position;
    float R, G, B, A;
};

struct FRay {
    FVector Origin;
    FVector Direction;
    FRay(FVector origin, FVector direction) : Origin(origin), Direction(direction) {};
    inline FVector GetNormalizedDirection() const { return Direction.GetSafeNormal(); }
};

struct FBoundingBox {
    FVector min;
    FVector max;
    FBoundingBox(FVector min, FVector max) : min(min), max(max) {};
    FVector GetGap() { return max - min; }
};

enum class EDepthComparisonMode
{
    Less,
    Always,
};

enum class ERasterizerMode
{
    Solid_None,
    Solid_Front,
    Solid_Back,
    Wireframe_None,
    Wireframe_Front,
    Wireframe_Back,
};


FString GetDepthComparisonModeAsString(enum class EDepthComparisonMode Mode);

FString GetERasterizerModeAsString(enum class ERasterizerMode Mode);
