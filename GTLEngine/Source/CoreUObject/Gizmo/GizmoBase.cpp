#include "pch.h"
#include "GizmoBase.h"
#include "Utils/Math/Geometry.h"

bool AGizmoBase::IsAbsoluteCoord = false;

AGizmoBase::AGizmoBase(): mat() {
}

void AGizmoBase::Init() {}

void AGizmoBase::Tick(float TickTime) {
    if ( IsAbsoluteCoord ) {
        mat = FMatrix::GetTranslateMatrix(Target->GetActorLocation());
    } else {
        mat = FMatrix();
        //mat = mat * FMatrix::GetScaleMatrix(Target->GetActorScale());
        mat = mat * FMatrix::GetRotateMatrix(Target->GetActorRotation());
        mat = mat * FMatrix::GetTranslateMatrix(Target->GetActorLocation());
    }
}

void AGizmoBase::Destroy() {}

FBoundingBox AGizmoBase::GetAABB() const {
    FVector min = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
    FVector max = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    FVector vecs[8];
    if ( Target == nullptr ) {
        OutputDebugString(L"AGizmoBase::GetAABB(): Target nullptr");
        return FBoundingBox(min, max);
    }

    for ( int i = 0; i < 8; i++ ) {
        vecs[i] = mat.TransformPositionVector(OriginalAABB[i]);
        if ( vecs[i].X < min.X ) min.X = vecs[i].X;
        if ( vecs[i].Y < min.Y ) min.Y = vecs[i].Y;
        if ( vecs[i].Z < min.Z ) min.Z = vecs[i].Z;
        if ( vecs[i].X > max.X ) max.X = vecs[i].X;
        if ( vecs[i].Y > max.Y ) max.Y = vecs[i].Y;
        if ( vecs[i].Z > max.Z ) max.Z = vecs[i].Z;
    }

    return FBoundingBox(min, max);
}
