#pragma once

#include "SceneComponent.h"
#include "Core/Resource/Types.h"

class UPrimitiveComponent : public USceneComponent
{
	DECLARE_CLASS(UPrimitiveComponent, USceneComponent)
public:
	UPrimitiveComponent();

	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:

	//virtual void Serialize(bool bIsLoading, json::JSON Handle);


	// 아래 사항은 Object 로드 방식으로 변경 시 삭제.
public:
	EPrimitiveType GetPrimitiveType() const { return PrimitiveType; }

	virtual FBoundingBox GetAABB() const override;
	virtual bool IsRayIntersect(FRay ray, float hitDistance, FVector& hitPoint) const override;

protected:
	EPrimitiveType PrimitiveType;
	FVector OriginalAABB[8] = {
		FVector(-0.5f, -0.5f, -0.5f),
		FVector(0.5f, -0.5f, -0.5f),
		FVector(-0.5f, 0.5f, -0.5f),
		FVector(0.5f, 0.5f, -0.5f),
		FVector(-0.5f, -0.5f, 0.5f),
		FVector(0.5f, -0.5f, 0.5f),
		FVector(-0.5f, 0.5f, 0.5f),
		FVector(0.5f, 0.5f, 0.5f),
	};



};