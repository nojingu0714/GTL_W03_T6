#pragma once

#include "Resource/Types.h"
#include <d3d11.h>
#include <d3dcompiler.h>

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

class UObject;
class AActor;
class ACamera;
class UGizmoBase;
class UGizmoManager;
class UPrimitiveComponent;
class ULineComponent;
class USceneComponent;
class FViewport;

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

	void UpdateCameraMatrix(ACamera* Camera);
	void RenderWorldPlane(ACamera* Camera);
	void RenderPrimitive(UPrimitiveComponent* PrimitiveComp, bool flag);
	void RenderBoundingBox(const TArray<AActor*> Actors);
	void RenderGizmo(const TArray<UGizmoBase*> Gizmos);
	void RenderObject(const TArray<AActor*> Actors);
	void RenderLines(const TArray<AActor*> Actors);
	void RenderActorUUID(AActor* TargetActor);
	void RenderComponentUUID(USceneComponent* TargetComponent);

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


	////////////////////////////////////////
	// Buffers
	// TODO: Name으로 버텍스 버퍼 저장.
	// Array 타입을 다른 방식으로 바꿔서 저장.
public:
	template<typename T>
	HRESULT AddVertexBuffer(FString KeyName, const TArray<T> vertices, const TArray<uint32>& indices);

	// for batch line render
	template<typename T>
	HRESULT CheckAndAddDynamicVertexBuffer(FString KeyName, const uint32 size);
	
	HRESULT AddConstantBuffer(EConstantBufferType Type);

	void ResizeViewport(int width, int height);
	HRESULT ResizeWindow(int width, int height);

private:
	void UpdateWorldViewMatrix(ACamera* Camera);
	void UpdateWorldProjectionMatrix(ACamera* Camera);

	void RenderAABB(FBoundingBox aabb);


public:
	void InitWindow(HWND hWnd, UINT InWidth, UINT InHeight);
	void PrepareWindow();
	void RenderWindow();
private:
	////////////////////////////////////////
	// Window Renderings (using swapchain's backbuffer)
	D3D11_VIEWPORT WindowViewport;
	//UDXDRenderTarget* WindowRenderTarget;
	//UDXDDepthStencilView* WindowDepthStencilView;
	//UDXDDepthStencilState* WindowDepthStencilState;


	////////////////////////////////////////
	// Viewport Renderings
public:
	void PrepareViewport(const FViewport& InViewport);
	//void RenderViewport(const FViewport& InViewport);

public:
	HRESULT AddRenderTarget(const FString& InName, const D3D11_TEXTURE2D_DESC InRenderTargetDesc, const D3D11_RENDER_TARGET_VIEW_DESC& InRenderTargetViewDesc = { DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, D3D11_RTV_DIMENSION_TEXTURE2D , 0 });
	HRESULT AddRenderTargetToSwapChain(const FString& InName);
	HRESULT AddDepthStencilView(const FString& InName, HWND hWnd, UINT InWidth, UINT InHeight);
	HRESULT AddDepthStencilState(const FString& InName, const D3D11_DEPTH_STENCIL_DESC& InDesc);
	HRESULT AddRasterizerState(const FString& InName, const D3D11_RASTERIZER_DESC& InDesc);

	UDXDRenderTarget* GetRenderTarget(const FString& InName);
	UDXDDepthStencilView* GetDepthStencilView(const FString& InName);
	UDXDDepthStencilState* GetDepthStencilStates(const FString& InName);
	UDXDRasterizerState* GetRasterizerState(const FString& InName);
	
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

	TMap<FString, FVertexInfo> VertexBuffers;
	TMap<FString, FIndexInfo> IndexBuffers;

	TMap<EConstantBufferType, UDXDConstantBuffer*> ConstantBuffers;

	// TODO: Texture 관리용 객체로 묶어서 관리.
	ID3D11ShaderResourceView* FontAtlasTexture;
	ID3D11SamplerState* FontSamplerState;
	FVertexInfo FontTextureBuffer;

	uint32 DynamicVertexBufferSize;

};

template<typename T>
inline HRESULT UDirectXHandle::AddVertexBuffer(FString KeyName, const TArray<T> vertices, const TArray<uint32>& indices)
{
	ID3D11Buffer* NewVertexBuffer;
	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(T) * static_cast<uint32>(vertices.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	HRESULT hr = DXDDevice->CreateBuffer(&bufferDesc, &initData, &NewVertexBuffer);
	if (FAILED(hr))
		return hr;

	FVertexInfo Info = { static_cast<uint32>(vertices.size()), NewVertexBuffer };
	VertexBuffers.insert({ KeyName, Info });

	if (indices.size() > 0)
	{
		ID3D11Buffer* NewIndexBuffer = nullptr;
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(uint32) * static_cast<uint32>(indices.size());
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA indexInitData = {};
		indexInitData.pSysMem = indices.data();

		hr = DXDDevice->CreateBuffer(&indexBufferDesc, &indexInitData, &NewIndexBuffer);
		if (FAILED(hr))
			return hr;

		FIndexInfo IndexInfo = { static_cast<uint32>(indices.size()), NewIndexBuffer };
		IndexBuffers.insert({ KeyName, IndexInfo });
	}

	return S_OK;
}

template<typename T>
inline HRESULT UDirectXHandle::CheckAndAddDynamicVertexBuffer(FString KeyName, const uint32 size) {
	
	if ( VertexBuffers.contains(KeyName) ) {

		if ( size < DynamicVertexBufferSize )
			return S_OK;

		while ( size >= DynamicVertexBufferSize ) {
			DynamicVertexBufferSize *= 2;
		}

		VertexBuffers[KeyName].VertexBuffer->Release();
		VertexBuffers[KeyName].VertexBuffer = nullptr;
	}
	
	ID3D11Buffer* NewVertexBuffer;
	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(T) * DynamicVertexBufferSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = DXDDevice->CreateBuffer(&bufferDesc, nullptr, &NewVertexBuffer);
	if ( FAILED(hr) )
		return hr;

	FVertexInfo Info = { 0, NewVertexBuffer };
	//VertexBuffers.insert({ KeyName, Info });
	VertexBuffers[KeyName] = Info;

	return S_OK;
}
