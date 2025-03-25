#pragma once
#include "Math/Matrix.h"
#include "Math/Rotator.h"
#include "Resource/Types.h"

struct FViewportCamera
{
	FVector Location;
	FRotator Rotation;
	EProjectionMode ProjectionMode;
	FMatrix CachedProjectionMatrix;
	FMatrix CachedViewMatrix;
	float ScreenSize; // Orthogonal
	float FieldOfView; // Perspective
	float NearClip;
	float FarClip;
	float Speed;
	float Sensitive;
	float MaxPitch;
	float MinPitch;

};

// 엔진에 TArray로 저장.
class FViewport
{
public:
	FViewport() = default;
	HRESULT Init(const FString& InName, HWND hWnd, int InX, int InY, UINT InWidth, UINT InHeight);
	void SetDepthcomparisonMode(EDepthComparisonMode InDepthComparisonMode);
	void SetRasterizerMode(ERasterizerMode InRasterizerMode);
	void MoveViewport(int InX, int InY);
	void ResizeViewport(UINT InWidth, UINT InHeight);
	void SetProjectionMatrix(const FMatrix& InProjectionMatrix);
	void SetViewMatrix(const FMatrix& InViewMatrix);

	void Tick(float TickTime);

	EDepthComparisonMode GetDepthComparisonMode() const { return DepthComparisonMode; }
	ERasterizerMode GetRasterizerMode() const { return RasterizerMode; }
	const D3D11_VIEWPORT& GetViewport() const { return Viewport; }
	const FString& GetName() const { return Name; }
	const FMatrix& GetProjectionMatrix() const { return Camera->CachedProjectionMatrix; }
	const FMatrix& GetViewMatrix() const { return Camera->CachedViewMatrix; }
	FViewportCamera* GetCamera() const { return Camera; }

	void Destroy();

private:
	FString Name;
	
	EDepthComparisonMode DepthComparisonMode = EDepthComparisonMode::Less;
	ERasterizerMode RasterizerMode = ERasterizerMode::Solid_Back;
	D3D11_VIEWPORT Viewport;
	FViewportCamera* Camera;
	
};