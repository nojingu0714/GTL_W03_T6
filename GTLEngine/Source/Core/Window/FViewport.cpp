#include <pch.h>
#include "FViewport.h"
#include "DirectX11/DirectXHandle.h"
#include "Engine/Engine.h"

bool FViewport::Init(const FString& InName, HWND hWnd, int InX, int InY, UINT InWidth, UINT InHeight)
{
	Name = InName;
	Viewport.TopLeftX = InX;
	Viewport.TopLeftY = InY;
	Viewport.Width = InWidth;
	Viewport.Height = InHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	
	UDirectXHandle* Handle = UEngine::GetEngine().GetDirectX11Handle();
	Handle->AddRenderTarget(InName);
	Handle->AddDepthStencilView(InName, hWnd, InWidth, InHeight);


}

void FViewport::SetDepthcomparisonMode(EDepthComparisonMode InDepthComparisonMode)
{
}

void FViewport::SetRasterizerMode(ERasterizerMode InRasterizerMode)
{
}

void FViewport::MoveViewport(int InX, int InY)
{
}

void FViewport::ResizeViewport(UINT InWidth, UINT InHeight)
{
}

void FViewport::SetProjectionMatrix(const FMatrix& InProjectionMatrix)
{
}
