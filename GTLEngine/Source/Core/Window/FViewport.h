#pragma once
#include "Math/Matrix.h"
#include "Resource/Types.h"

// 엔진에 TArray로 저장.
class FViewport
{
public:
	FViewport() = default;
	bool Init(const FString& InName, HWND hWnd, int InX, int InY, UINT InWidth, UINT InHeight);
	void SetDepthcomparisonMode(EDepthComparisonMode InDepthComparisonMode);
	void SetRasterizerMode(ERasterizerMode InRasterizerMode);
	void MoveViewport(int InX, int InY);
	void ResizeViewport(UINT InWidth, UINT InHeight);
	void SetProjectionMatrix(const FMatrix& InProjectionMatrix);


	EDepthComparisonMode GetDepthComparisonMode() const { return DepthComparisonMode; }
	ERasterizerMode GetRasterizerMode() const { return RasterizerMode; }
	//const D3D11_VIEWPORT& GetViewport() const { return Viewport; }
	const FString& GetName() const { return Name; }
	const FMatrix& GetProjectionMatrix() const { return CachedProjectionMatrix; }


private:
	FString Name;

	FMatrix CachedProjectionMatrix;
	EDepthComparisonMode DepthComparisonMode = EDepthComparisonMode::Less;
	ERasterizerMode RasterizerMode = ERasterizerMode::Solid_Back;
	D3D11_VIEWPORT Viewport;
};