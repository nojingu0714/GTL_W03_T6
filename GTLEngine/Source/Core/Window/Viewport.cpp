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

	// 컬러 버퍼.
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
	Camera = new FViewportCamera();
	Camera->Location = FVector(-10.f, 0.f, 0.0f);
	Camera->Rotation = FRotator(0.f, 0.f, 0.f);
	Camera->ProjectionMode = EProjectionMode::Perspective;
	Camera->ScreenSize = 0.f;
	Camera->FieldOfView = 60.f;
	Camera->NearClip = 0.1f;
	Camera->FarClip = 1000.f;
	Camera->Speed = 10.f;
	Camera->Sensitive = 5.f;
	Camera->MaxPitch = 89.f;
	Camera->MinPitch = -89.f;

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc = {};
	RenderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = Handle->AddRenderTarget(InName, TextureDesc, RenderTargetViewDesc);
	if (FAILED(hr))
	{
		UE_LOG(LogTemp, Warning, TEXT("FViewport::Init::Failed to add render target"));
		return hr;
	}
	hr = Handle->AddDepthStencilView(InName, hWnd, InWidth, InHeight);
	if (FAILED(hr))
	{
		UE_LOG(LogTemp, Warning, TEXT("FViewport::Init::Failed to add render target"));
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
	Camera->CachedProjectionMatrix = InProjectionMatrix;
}

void FViewport::SetViewMatrix(const FMatrix& InViewMatrix)
{
	Camera->CachedViewMatrix = InViewMatrix;
}

void FViewport::Tick(float DeltaTime)
{
	FVector CameraLocation = Camera->Location;
	FRotator CameraRotation = Camera->Rotation;
	
	// ViewMatrix Update
	// Rotation Matrix 생성.
	FVector ForwardVector = CameraRotation.TransformRotVecToMatrix(FVector::ForwardVector).GetSafeNormal();

	//XMMatrixLookAtLH(Eye, At, Up);
	FMatrix CameraViewMatrix = FMatrix::LookAtLH(CameraLocation, CameraLocation + ForwardVector, FVector::UpVector);
	Camera->CachedViewMatrix = CameraViewMatrix;

	float FOVRad = FMath::DegreesToRadians(Camera->FieldOfView);
	// TODO: resize / fov / nearfar 변경되었을때만 변경할 수 있도록 저장.
	Camera->CachedProjectionMatrix = FMatrix::PerspectiveFovLH(FOVRad, Viewport.Width / Viewport.Height, Camera->NearClip, Camera->FarClip);

}

void FViewport::TickWhenSelected(float DeltaTime)
{

}

void FViewport::TickWhenHovered(float DeltaTime)
{

}

void FViewport::ProcessInput(float DeltaTime)
{
	// 카메라 업데이트
	FVector CameraLocation = Camera->Location;
	FRotator CameraRotation = Camera->Rotation;

	// 카메라 기준 Forward, Right, Up Vector 구하기

	FVector ForwardDirection = CameraRotation.TransformRotVecToMatrix(FVector::ForwardVector);
	FVector RightDirection = CameraRotation.TransformRotVecToMatrix(FVector::RightVector);
	FVector UpDirection = CameraRotation.TransformRotVecToMatrix(FVector::UpVector);

	UInputManager* InputManager = UEngine::GetEngine().GetInputManager();

	if (InputManager->GetKey('W'))
	{
		CameraLocation += ForwardDirection * Camera->Speed * DeltaTime;
	}
	if (InputManager->GetKey('S'))
	{
		CameraLocation -= ForwardDirection * Camera->Speed * DeltaTime;
	}
	if (InputManager->GetKey('A'))
	{
		CameraLocation -= RightDirection * Camera->Speed * DeltaTime;
	}
	if (InputManager->GetKey('D'))
	{
		CameraLocation += RightDirection * Camera->Speed * DeltaTime;
	}
	if (InputManager->GetKey('Q'))
	{
		CameraLocation -= UpDirection * Camera->Speed * DeltaTime;
	}
	if (InputManager->GetKey('E'))
	{
		CameraLocation += UpDirection * Camera->Speed * DeltaTime;
	}
	if (InputManager->GetMouseButton(UInputManager::EMouseButton::RIGHT))
	{
		float MouseDeltaX = static_cast<float>(InputManager->GetMouseDeltaX());
		float MouseDeltaY = static_cast<float>(InputManager->GetMouseDeltaY());

		// Pitch, Yaw, Roll == Y, Z, X
		// TODO: 회전 시 Roll 회전이 적용되는 문제가 생김. Rotator 문제일 수도 있음.

		CameraRotation.Pitch -= MouseDeltaY * Camera->Sensitive * DeltaTime;
		CameraRotation.Yaw += MouseDeltaX * Camera->Sensitive * DeltaTime;

		CameraRotation.Pitch = (CameraRotation.Pitch < Camera->MinPitch) ? Camera->MinPitch : (CameraRotation.Pitch > Camera->MaxPitch) ? Camera->MaxPitch : CameraRotation.Pitch;
		CameraRotation.Roll = 0;
	}

	Camera->Location = CameraLocation;
	Camera->Rotation = CameraRotation;
}

void FViewport::Destroy()
{
	delete Camera;
	Camera = nullptr;
}

void FViewport::GetRayOnWorld(int InScreenMouseX, int InScreenMouseY, FVector& OutRayOriginOnWorld, FVector& OutRayDirOnWorld)
{
	float NDCMouseX = (InScreenMouseX - Viewport.TopLeftX) / (float)Viewport.Width * 2 - 1;
	float NDCMouseY = (InScreenMouseY - Viewport.TopLeftY) / (float)Viewport.Height * -2 + 1;

	FVector4 NDCRayNear = FVector4(NDCMouseX, NDCMouseY, 0, 1);
	FVector4 NDCRayFar = FVector4(NDCMouseX, NDCMouseY, 1, 1);

	// Clip -> (ProjMat).Inv -> View -> (View).Inv -> World
	FMatrix ViewProjInv = (Camera->CachedViewMatrix * Camera->CachedProjectionMatrix).Inverse();

	FVector4 WorldRayNear = ViewProjInv.TransformVector4(NDCRayNear);
	FVector4 WorldRayFar = ViewProjInv.TransformVector4(NDCRayFar);

	WorldRayNear /= WorldRayNear.W;
	WorldRayFar /= NDCRayFar.W;

	OutRayOriginOnWorld = WorldRayNear.xyz();
	OutRayDirOnWorld = (WorldRayFar - WorldRayNear).xyz();
}

bool FViewport::Contains(int x, int y) const
{
	return(Viewport.TopLeftX <= x && x <= Viewport.TopLeftX + Viewport.Width &&
		Viewport.TopLeftY <= y && y <= Viewport.TopLeftY + Viewport.Height);
}
