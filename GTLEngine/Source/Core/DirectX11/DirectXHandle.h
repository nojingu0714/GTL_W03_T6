#pragma once

#include "Resource/Types.h"
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Asset/Mesh/StaticMeshAsset.h"

class UDXDDevice;
class UDXDSwapChain;
class UDXDRenderTarget;
class UDXDDepthStencilView;
class UDXDDepthStencilState;
class UDXDRasterizerState;
class UDXDShaderManager;
class UDXDInputLayout;
class UDXDConstantBuffer;
class UDXDBufferManager;
class UDXDResourceManager;

class UObject;
class AActor;
class ACamera;
class AGizmoBase;
class FGizmoManager;
class UPrimitiveComponent;
class ULineComponent;
class USceneComponent;
class UStaticMeshComponent;
class FViewport;

struct FViewportCamera;

class UDirectXHandle
{
public:
	UDirectXHandle() = default;
	~UDirectXHandle();

private:
	HRESULT CreateDeviceAndSwapchain();
	HRESULT CreateShaderManager();

public:
	HRESULT CreateDirectX11Handle(HWND hWnd);
	void ReleaseDirectX11Handle();

	void UpdateCameraMatrix(FViewportCamera* Camera);
	void RenderWorldPlane(const FViewportCamera* Camera);
	void RenderWorldXYZAxis();
	void RenderBoundingBox(const TArray<AActor*> Actors);
	void RenderGizmo(AGizmoBase* Gizmo);
	void RenderStaticMesh(UStaticMeshComponent* Comp);
	void RenderObject(const TArray<AActor*> Actors);
	void RenderActorUUID(AActor* TargetActor);

public:
	//void InitView();

public:
	inline void SetLineMode() { DXDDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); }
	inline void SetFaceMode() { DXDDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); }

private:


	////////////////////////////////////////
	// D3D11 / DXGI 
public:
	ID3D11Device*	GetD3DDevice() const { return DXDDevice; }
	ID3D11DeviceContext* GetD3DDeviceContext() const { return DXDDeviceContext; }
	IDXGISwapChain* GetDXDSwapChain() const { return DXDSwapChain; }

private:
	ID3D11Device* DXDDevice;
	ID3D11DeviceContext* DXDDeviceContext;
	IDXGISwapChain* DXDSwapChain;

public:
	HRESULT AddConstantBuffer(EConstantBufferType Type);

	void ResizeViewport(int width, int height);
	HRESULT ResizeWindow(int width, int height);

private:
	void RenderAABB(FBoundingBox InBox);


public:
	void InitWindow(HWND hWnd, UINT InWidth, UINT InHeight);
	void PrepareWindow();
	void RenderWindow();
private:
	////////////////////////////////////////
	// Window Renderings (using swapchain's backbuffer)
	D3D11_VIEWPORT WindowViewport;

	////////////////////////////////////////
	// Viewport Renderings
public:
	void PrepareViewport(FViewport& InViewport);
	void RenderViewport(FViewport& InViewport, bool isDepthStencil = false);
	void EndRenderViewport();

private:
	HRESULT UpdateViewportBuffer(const FViewport& InViewport);

public:
	HRESULT AddRenderTarget(const FString& InName, const D3D11_TEXTURE2D_DESC InRenderTargetDesc, const D3D11_RENDER_TARGET_VIEW_DESC& InRenderTargetViewDesc);
	HRESULT AddRenderTargetToSwapChain(const FString& InName);
	HRESULT AddDepthStencilView(const FString& InName, UINT InWidth, UINT InHeight);
	HRESULT AddDepthStencilState(const FString& InName, const D3D11_DEPTH_STENCIL_DESC& InDesc);
	HRESULT AddRasterizerState(const FString& InName, const D3D11_RASTERIZER_DESC& InDesc);

	UDXDRenderTarget* GetRenderTarget(const FString& InName);
	UDXDDepthStencilView* GetDepthStencilView(const FString& InName);
	UDXDDepthStencilState* GetDepthStencilState(const FString& InName);
	UDXDRasterizerState* GetRasterizerState(const FString& InName);

	HRESULT ReleaseRenderTarget(const FString& InName);
	HRESULT ReleaseDepthStencilView(const FString& InName);
	
	UDXDShaderManager* GetShaderManager() const { return ShaderManager; }

private:
	// TMap으로 관리
	TMap<FString, UDXDRenderTarget*> RenderTargets; // viewport당 하나
	TMap<FString, UDXDDepthStencilView*> DepthStencilViews; // viewport당 하나
	TMap<FString, UDXDDepthStencilState*> DepthStencilStates; // 특정 오브젝트당 하나 : 깊이비교 테스트
	TMap<FString, UDXDRasterizerState*> RasterizerStates; // 특정 오브젝트당 하나 : 와이퍼프레임 or 솔리드
	
	////////////////////////////////////////
	// Resources
	UDXDShaderManager* ShaderManager;
	UDXDBufferManager* BufferManager;
	UDXDResourceManager* ResourceManager;

	TMap<EConstantBufferType, UDXDConstantBuffer*> ConstantBuffers;


	uint32 DynamicVertexBufferSize;


	////////////////////////////////////////
	// Render Debugging
public:
	void RenderDebugRays(const TArray<FRay>& Rays);
};
