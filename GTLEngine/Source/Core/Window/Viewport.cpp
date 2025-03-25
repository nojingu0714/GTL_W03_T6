#include <pch.h>
#include "Viewport.h"
#include "DirectX11/DirectXHandle.h"
#include "Engine/Engine.h"
#include "Input/InputManager.h"


HRESULT FViewport::Init(const FString& InName, HWND hWnd, int InX, int InY, UINT InWidth, UINT InHeight)
{
	Name = InName;
	Viewport.TopLeftX = InX;
	Viewport.TopLeftY = InY;
	Viewport.Width = InWidth;
	Viewport.Height = InHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	
	UDirectXHandle* Handle = UEngine::GetEngine().GetDirectX11Handle();
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC TextureDesc = {};
	TextureDesc.Width = InWidth;
	TextureDesc.Height = InHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	// 카메라 초기화
	Camera.Location = FVector(0.f, 2.f, 0.f);
	Camera.Rotation = FRotator(0.f, 0.f, 0.f);
	Camera.ProjectionMode = EProjectionMode::Perspective;
	Camera.ScreenSize = 0.f;
	Camera.FieldOfView = 60.f;
	Camera.NearClip = 0.1f;
	Camera.FarClip = 1000.f;
	Camera.Speed = 10.f;
	Camera.Sensitive = 5.f;
	Camera.MaxPitch = 89.f;
	Camera.MinPitch = -89.f;

	hr = Handle->AddRenderTarget(InName, TextureDesc);
	if (FAILED(hr))
	{
		UE_LOG(TEXT("FViewport::Init::Failed to add render target"));
		return hr;
	}
	hr = Handle->AddDepthStencilView(InName, hWnd, InWidth, InHeight);
	if (FAILED(hr))
	{
		UE_LOG(TEXT("FViewport::Init::Failed to add render target"));
		return hr;
	}
	return hr;
}

void FViewport::SetDepthcomparisonMode(EDepthComparisonMode InDepthComparisonMode)
{
	DepthComparisonMode = InDepthComparisonMode;
}

void FViewport::SetRasterizerMode(ERasterizerMode InRasterizerMode)
{
	RasterizerMode = InRasterizerMode;
}

void FViewport::MoveViewport(int InX, int InY)
{
	Viewport.TopLeftX = InX;
	Viewport.TopLeftY = InY;
}

void FViewport::ResizeViewport(UINT InWidth, UINT InHeight)
{
	Viewport.Width = InWidth;
	Viewport.Height = InHeight;
}

void FViewport::SetProjectionMatrix(const FMatrix& InProjectionMatrix)
{
	CachedProjectionMatrix = InProjectionMatrix;
}

void FViewport::SetViewMatrix(const FMatrix& InViewMatrix)
{
	CachedViewMatrix = InViewMatrix;
}

void FViewport::Tick(float TickTime)
{
	// 카메라 업데이트
	FVector CameraLocation = Camera.Location;
	FRotator CameraRotation = Camera.Rotation;

	// 카메라 기준 Forward, Right, Up Vector 구하기

	FVector ForwardDirection = CameraRotation.TransformRotVecToMatrix(FVector::ForwardVector);
	FVector RightDirection = CameraRotation.TransformRotVecToMatrix(FVector::RightVector);
	FVector UpDirection = CameraRotation.TransformRotVecToMatrix(FVector::UpVector);

	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();

	if (InputManager->GetKey('W'))
	{
		CameraLocation += ForwardDirection * Camera.Speed * TickTime;
	}
	if (InputManager->GetKey('S'))
	{
		CameraLocation -= ForwardDirection * Camera.Speed * TickTime;
	}
	if (InputManager->GetKey('A'))
	{
		CameraLocation -= RightDirection * Camera.Speed * TickTime;
	}
	if (InputManager->GetKey('D'))
	{
		CameraLocation += RightDirection * Camera.Speed * TickTime;
	}
	if (InputManager->GetKey('Q'))
	{
		CameraLocation -= UpDirection * Camera.Speed * TickTime;
	}
	if (InputManager->GetKey('E'))
	{
		CameraLocation += UpDirection * Camera.Speed * TickTime;
	}
	if (InputManager->GetMouseButton(UInputManager::EMouseButton::RIGHT))
	{
		float MouseDeltaX = static_cast<float>(InputManager->GetMouseDeltaX());
		float MouseDeltaY = static_cast<float>(InputManager->GetMouseDeltaY());

		// Pitch, Yaw, Roll == Y, Z, X
		// TODO: 회전 시 Roll 회전이 적용되는 문제가 생김. Rotator 문제일 수도 있음.

		CameraRotation.Pitch -= MouseDeltaY * Camera.Sensitive * TickTime;
		CameraRotation.Yaw += MouseDeltaX * Camera.Sensitive * TickTime;

		CameraRotation.Pitch = (CameraRotation.Pitch < Camera.MinPitch) ? Camera.MinPitch : (CameraRotation.Pitch > Camera.MaxPitch) ? Camera.MaxPitch : CameraRotation.Pitch;
		CameraRotation.Roll = 0;
	}

	Camera.Location = CameraLocation;
	Camera.Rotation = CameraRotation;
}
