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

struct FVertexPC
{
    FVector Position;
    FVector4 Color;
};

struct FVertexPT
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
    ViewportRatio,
    Material,
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

struct FCbMaterial
{
    FVector Ambient;
    float Opacity;
    FVector Diffuse;
    float Transparency;
    FVector Specular;
    float RefractiveIndex;
    FVector Emissive;
    float SpecularExponent;
    int IlluminationModel;
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
    FBoundingBox() : min({ FLT_MAX, FLT_MAX ,FLT_MAX }), max({ -FLT_MAX, -FLT_MAX ,-FLT_MAX }) {}
    FBoundingBox(FVector min, FVector max) : min(min), max(max) {};
    FVector GetGap() { return max - min; }
    void GetCenterAndExtents(FVector& Center, FVector& Extents) const
    {
        Center = (min + max) * 0.5f;
        Extents = (max - min) * 0.5f;
    }

    inline FBoundingBox& operator+=(const FBoundingBox& Other)
    {
        min.X = std::min(min.X, Other.min.X);
        min.Y = std::min(min.Y, Other.min.Y);
        min.Z = std::min(min.Z, Other.min.Z);
        max.X = std::max(max.X, Other.max.X);
        max.Y = std::max(max.Y, Other.max.Y);
        max.Z = std::max(max.Z, Other.max.Z);
        return *this;
    }
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

enum class EProjectionMode
{
	Perspective,
	Orthogonal,
};

FString GetDepthComparisonModeAsString(enum class EDepthComparisonMode Mode);

FString GetERasterizerModeAsString(enum class ERasterizerMode Mode);
