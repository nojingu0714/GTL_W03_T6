#include "pch.h"
#include "Camera.h"

#include "Components/CameraComponent.h"
#include "Components/LineComponent.h"
#include "Engine/Engine.h"
#include "World.h"
#include "Resource/ResourceManager.h"
#include "Input/InputManager.h"

#include "Math/MathUtils.h"
#include "Utils/Math/Geometry.h"

ACamera::ACamera() : MouseSensitive(5.f)
{
	CameraComponent = AddComponent<UCameraComponent>(this);
	CameraComponent->SetupAttachment(RootComponent);
	SetActorLocation(FVector(0, 0, 0));
	SetActorRotation(FVector(0, 0, 0));
	SetActorScale(FVector(1, 1, 1));

	UResourceManager* resourceManager = UEngine::GetEngine().GetResourceManager();
	GridScale = resourceManager->GetConfigData(EConfigData::GridScale, 1.0f);
	MouseSensitive = resourceManager->GetConfigData(EConfigData::MouseSensitive, 5.0f);
	MoveSpeed = resourceManager->GetConfigData(EConfigData::MoveSpeed, 10.f);
}

void ACamera::Tick(float TickTime)
{
	Super::Tick(TickTime);
	FVector CameraLocation = GetActorLocation();
	FRotator CameraRotation = GetActorRotation();

	// 현재 카메라 회전을 기준으로 바꿔줘야 함.
	FRotator Forward = GetActorRotation();
	// 카메라 기준 Forward, Right, Up Vector 구하기
	
	FVector ForwardDirection = CameraRotation.TransformRotVecToMatrix(FVector::ForwardVector);
	FVector RightDirection = CameraRotation.TransformRotVecToMatrix(FVector::RightVector);
	FVector UpDirection = CameraRotation.TransformRotVecToMatrix(FVector::UpVector);

	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();
	
	if (InputManager->GetKey('W'))
	{
		CameraLocation += ForwardDirection * MoveSpeed * TickTime;
	}
	if (InputManager->GetKey('S'))
	{
		CameraLocation -= ForwardDirection * MoveSpeed * TickTime;
	}
	if (InputManager->GetKey('A'))
	{
		CameraLocation -= RightDirection * MoveSpeed * TickTime;
	}
	if (InputManager->GetKey('D'))
	{
		CameraLocation += RightDirection * MoveSpeed * TickTime;
	}
	if ( InputManager->GetKey('Q') ) 
	{
		CameraLocation -= UpDirection * MoveSpeed * TickTime;
	}
	if ( InputManager->GetKey('E') ) {
		CameraLocation += UpDirection * MoveSpeed * TickTime;
	}
	if (InputManager->GetMouseButton(UInputManager::EMouseButton::RIGHT))
	{
		float MouseDeltaX = static_cast<float>(InputManager->GetMouseDeltaX());
		float MouseDeltaY = static_cast<float>(InputManager->GetMouseDeltaY());

		// Pitch, Yaw, Roll == Y, Z, X
		// TODO: 회전 시 Roll 회전이 적용되는 문제가 생김. Rotator 문제일 수도 있음.

		CameraRotation.Pitch -= MouseDeltaY * MouseSensitive * TickTime;
		CameraRotation.Yaw += MouseDeltaX * MouseSensitive * TickTime;

		float MaxPitch = (89.0f);
		float MinPitch = (-89.0f);
		CameraRotation.Pitch = (CameraRotation.Pitch < MinPitch) ? MinPitch : (CameraRotation.Pitch > MaxPitch) ? MaxPitch : CameraRotation.Pitch;
		CameraRotation.Roll = 0;
	}

	SetActorLocation(CameraLocation);
	SetActorRotation(CameraRotation);
}

void ACamera::Destroy()
{
	Super::Destroy();
	SaveConfig();
}

void ACamera::SaveConfig() {
	UResourceManager* resourceManager = UEngine::GetEngine().GetResourceManager();
	resourceManager->SetConfigData(EConfigData::GridScale, GridScale);
	resourceManager->SetConfigData(EConfigData::MouseSensitive, MouseSensitive);
	resourceManager->SetConfigData(EConfigData::MoveSpeed, MoveSpeed);
}

float ACamera::GetFieldOfView() const
{
	if (!CameraComponent)
		return 0.0f;
	return CameraComponent->GetFieldOfView();
}

float ACamera::GetNearClip() const
{
	if (!CameraComponent)
		return 0.0f;
	return CameraComponent->GetNearClip();
}

float ACamera::GetFarClip() const
{
	if (!CameraComponent)
		return 0.0f;
	return CameraComponent->GetFarClip();
}

