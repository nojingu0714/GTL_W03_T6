#include "pch.h"
#include "PrimitiveComponent.h"
#include "Core/Engine/Engine.h"
#include "Core/Resource/ResourceManager.h"
#include "Utils/Math/Geometry.h"

UPrimitiveComponent::UPrimitiveComponent()
	: USceneComponent(), PrimitiveType(EPrimitiveType::None)
{
}

void UPrimitiveComponent::TickComponent(float TickTime)
{
}

void UPrimitiveComponent::Destroy()
{
}

void UPrimitiveComponent::Serialize()
{

}

FBoundingBox UPrimitiveComponent::GetAABB(const FMatrix& LocalToWorld) const {
    FVector min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
    FVector max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    FVector vecs[8];

    FMatrix transform = GetWorldMatrix();
    for ( int i = 0; i < 8; i++ ) {
        vecs[i] = transform.TransformPositionVector(OriginalAABB[i]);
        if ( vecs[i].X < min.X ) min.X = vecs[i].X;
        if ( vecs[i].Y < min.Y ) min.Y = vecs[i].Y;
        if ( vecs[i].Z < min.Z ) min.Z = vecs[i].Z;
        if ( vecs[i].X > max.X ) max.X = vecs[i].X;
        if ( vecs[i].Y > max.Y ) max.Y = vecs[i].Y;
        if ( vecs[i].Z > max.Z ) max.Z = vecs[i].Z;
    }

    return FBoundingBox(min, max);
}

bool UPrimitiveComponent::IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const {
    FMatrix transform = GetWorldMatrix().Inverse();
    FRay transformedRay = FRay(transform.TransformPositionVector(ray.Origin), transform.TransformDirectionVector(ray.Direction).GetSafeNormal());

    //TArray<FVertexSimple> vertices = UEngine::GetEngine().GetResourceManager()->GetPrimitiveVertexData(PrimitiveType);
    //int nIntersections = 0;
    //if ( vertices.size() < 3 )
    //    return false;

    bool result = false;
    ////BYTE* pbPositions = reinterpret_cast<BYTE*>(vertices.data());

    //uint32 nPrimitives = static_cast<uint32>(vertices.size() / 3);

    //float nearHitDistancePow = FLT_MAX;
    //FVector nearestHitPoint = FVector::Zero();
    //for ( uint32 i = 0; i < nPrimitives; i++ ) {
    //    int idx0, idx1, idx2;
    //    idx0 = i * 3;
    //    idx1 = i * 3 + 1;
    //    idx2 = i * 3 + 2;

    //    FVector v0 = FVector(vertices[idx0].X, vertices[idx0].Y, vertices[idx0].Z);
    //    FVector v1 = FVector(vertices[idx1].X, vertices[idx1].Y, vertices[idx1].Z);
    //    FVector v2 = FVector(vertices[idx2].X, vertices[idx2].Y, vertices[idx2].Z);

    //    FVector hit;
    //    if ( Geometry::IsRayIntersectWithTriangle(transformedRay, v0, v1, v2, hitDistance, hit) ) {
    //        if ( (hitPoint - transformedRay.Origin).LengthSquared() < nearHitDistancePow ) {
    //            nearHitDistancePow = (hitPoint - transformedRay.Origin).LengthSquared();
    //            hitPoint = hit;
    //        }
    //        result = true;
    //    }
    //}
    return result;
}
