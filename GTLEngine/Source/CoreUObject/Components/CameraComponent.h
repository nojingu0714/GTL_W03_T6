#pragma once

#include "Components/SceneComponent.h"

class UCameraComponent : public USceneComponent
{
	DECLARE_CLASS(UCameraComponent, USceneComponent)
public:
	UCameraComponent();
	virtual void TickComponent(float TickTime) override;
	virtual void Destroy() override;

public:
	float GetFieldOfView() const { return FieldOfView; }
	float GetNearClip() const { return NearClip; }
	float GetFarClip() const { return FarClip; }

private:
	float FieldOfView;
	float NearClip;
	float FarClip;

};