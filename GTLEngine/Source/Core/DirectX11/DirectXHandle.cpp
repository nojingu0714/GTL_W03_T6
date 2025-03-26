#include "pch.h"
#include "DirectXHandle.h"

#include "DXDRenderTarget.h"
#include "DXDDepthStencilView.h"
#include "DXDRasterizerState.h"
#include "DXDShaderManager.h"
#include "DXDConstantBuffer.h"
#include "State/DXDDepthStencilState.h"
#include "DXDBufferManager.h"
#include "DXDResourceManager.h"

#include "CoreUObject/GameFrameWork/Actor.h"
#include "CoreUObject/GameFrameWork/Camera.h"

#include "Gizmo/GizmoActor.h"
#include "Gizmo/GizmoTranslateComponent.h"
#include "Gizmo/GizmoRotateComponent.h"
#include "Gizmo/GizmoScaleComponent.h"

#include "Mesh/UStaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Asset/ObjManager.h"
#include "Asset/Material.h"
#include "Asset/Texture.h"

#include "CoreUObject/Components/CameraComponent.h"

#include "Engine.h"

#include "Gizmo/GizmoManager.h"
#include "World.h"

#include "Math/Matrix.h"

#include "Window/Viewport.h"
#include "Core/Window/ViewportClient.h"

HRESULT CreatePrimitives(UDirectXHandle* Handle);

UDirectXHandle::~UDirectXHandle()
{
	ReleaseDirectX11Handle();
}

HRESULT UDirectXHandle::CreateDeviceAndSwapchain()
{
	D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    swapchaindesc.BufferDesc.Width = UEngine::GetEngine().GetWindowInfo().Width; // 창 크기에 맞게 자동으로 설정
    swapchaindesc.BufferDesc.Height = UEngine::GetEngine().GetWindowInfo().Height; // 창 크기에 맞게 자동으로 설정
    swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 색상 포맷
    swapchaindesc.SampleDesc.Count = 1; // 멀티 샘플링 비활성화
    swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟으로 사용
    swapchaindesc.BufferCount = 2; // 더블 버퍼링
    swapchaindesc.OutputWindow = UEngine::GetEngine().GetWindowInfo().WindowHandle; // 렌더링할 창 핸들
    swapchaindesc.Windowed = TRUE; // 창 모드
    swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 스왑 방식

	uint CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

	// Direct3D 장치와 스왑 체인을 생성
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceFlags, FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION, &swapchaindesc, &DXDSwapChain, &DXDDevice, nullptr, &DXDDeviceContext);

	if (FAILED(hr))
		return hr;

	// 생성된 스왑 체인의 정보 가져오기
	DXDSwapChain->GetDesc(&swapchaindesc);

	return S_OK;
}

HRESULT UDirectXHandle::CreateShaderManager()
{
	ShaderManager = new UDXDShaderManager(DXDDevice);
	if (ShaderManager == nullptr)
		return S_FALSE;

    // Primitive VS, PS, InputLayout 생성.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    HRESULT hr = ShaderManager->AddVertexShaderAndInputLayout(L"DefaultVS", L"Resource/Shader/StaticMeshShader.hlsl", "mainVS", layout, ARRAYSIZE(layout));
    if (FAILED(hr))
        return hr;

    hr = ShaderManager->AddPixelShader(L"DefaultPS", L"Resource/Shader/StaticMeshShader.hlsl", "mainPS");
    if (FAILED(hr))
        return hr;

    // Texture VS, PS, InputLayout 생성.
    D3D11_INPUT_ELEMENT_DESC TextureLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  offsetof(FVertexPT, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(FVertexPT, UV), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = ShaderManager->AddVertexShaderAndInputLayout(L"FontVS", L"Resource/Shader/FontShader.hlsl", "mainVS", TextureLayout, ARRAYSIZE(TextureLayout));
    if (FAILED(hr))
        return hr;

    hr = ShaderManager->AddPixelShader(L"FontPS", L"Resource/Shader/FontShader.hlsl", "mainPS");
    if (FAILED(hr))
        return hr;

	// Viewport VS, PS, InputLayout 생성.
	D3D11_INPUT_ELEMENT_DESC ViewportLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = ShaderManager->AddVertexShaderAndInputLayout(L"ViewportVS", L"Resource/Shader/ViewportShader.hlsl", "mainVS", ViewportLayout, ARRAYSIZE(ViewportLayout));
	if (FAILED(hr))
		return hr;

	hr = ShaderManager->AddPixelShader(L"ViewportPS", L"Resource/Shader/ViewportShader.hlsl", "mainPS");
	if (FAILED(hr))
		return hr;

	// Ray용 VS, PS, InputLayout 생성.
	D3D11_INPUT_ELEMENT_DESC LineLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = ShaderManager->AddVertexShaderAndInputLayout(L"LineVS", L"Resource/Shader/PrimitiveShader.hlsl", "mainVS", LineLayout, ARRAYSIZE(ViewportLayout));
	if (FAILED(hr))
		return hr;

	hr = ShaderManager->AddPixelShader(L"LinePS", L"Resource/Shader/PrimitiveShader.hlsl", "mainPS");
	if (FAILED(hr))
		return hr;

	// Color override(Gizmo)용 VS, PS, InputLayout 생성.

	hr = ShaderManager->AddVertexShaderAndInputLayout(L"GizmoVS", L"Resource/Shader/GizmoShader.hlsl", "mainVS", layout, ARRAYSIZE(layout));
	if (FAILED(hr))
		return hr;

	hr = ShaderManager->AddPixelShader(L"GizmoPS", L"Resource/Shader/GizmoShader.hlsl", "mainPS");
	if (FAILED(hr))
		return hr;


    return S_OK;
}

HRESULT UDirectXHandle::CreateDirectX11Handle(HWND hWnd)
{
	HRESULT hr;
	// 디바이스 및 스왑체인 생성.
	hr = CreateDeviceAndSwapchain();
	if (FAILED(hr))
		return hr;

	// 래스터라이저 생성.
	D3D11_RASTERIZER_DESC NormalRasterizerDesc = {};
	NormalRasterizerDesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드.
	NormalRasterizerDesc.CullMode = D3D11_CULL_BACK; // 백 페이스 컬링.
	AddRasterizerState(TEXT("Default"), NormalRasterizerDesc);

	D3D11_RASTERIZER_DESC GizmoRasterizerDesc = {};
	GizmoRasterizerDesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드.
	GizmoRasterizerDesc.CullMode = D3D11_CULL_BACK; // 백 페이스 컬링.
	AddRasterizerState(TEXT("Gizmo"), GizmoRasterizerDesc);

	D3D11_RASTERIZER_DESC WireframeRasterizerDesc = {};
	WireframeRasterizerDesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
	WireframeRasterizerDesc.CullMode = D3D11_CULL_BACK; // 백 페이스 컬링
	AddRasterizerState(TEXT("Wireframe"), WireframeRasterizerDesc);

	// 셰이더 초기화. VertexShader, PixelShader, InputLayout 생성.
	// VertexShader, InputLayout 는 쌍으로 생성 및 이름으로 관리.
	hr = CreateShaderManager();

	// 상수 버퍼 생성 및 버텍스 쉐이더와 픽셀 쉐이더에 바인딩
	for (uint32 i = 0; i < static_cast<uint32>(EConstantBufferType::Max); i++)
	{
		EConstantBufferType Type = static_cast<EConstantBufferType>(i);
		if (Type == EConstantBufferType::None)
		{
			continue;
		}

		hr = AddConstantBuffer(Type);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// 쉐이더에 상수 버퍼 바인딩
	if (UDXDConstantBuffer* DXDCB = ConstantBuffers[EConstantBufferType::ChangesOnResize])
	{
		if (ID3D11Buffer* CbChangedOnResize = DXDCB->GetConstantBuffer())
		{
			DXDDeviceContext->VSSetConstantBuffers(0, 1, &CbChangedOnResize);
		}
	}
	if (UDXDConstantBuffer* DXDCB = ConstantBuffers[EConstantBufferType::ChangesEveryFrame])
	{
		if (ID3D11Buffer* CbChangesEveryFrame = DXDCB->GetConstantBuffer())
		{
			DXDDeviceContext->VSSetConstantBuffers(1, 1, &CbChangesEveryFrame);
		}
	}
    if (UDXDConstantBuffer* DXDCB = ConstantBuffers[EConstantBufferType::ChangesEveryObject])
    {
        if (ID3D11Buffer* CbChangesEveryObject = DXDCB->GetConstantBuffer())
        {
            DXDDeviceContext->VSSetConstantBuffers(2, 1, &CbChangesEveryObject);
        }
    }
	if (UDXDConstantBuffer* DXDCB = ConstantBuffers[EConstantBufferType::ViewportRatio])
	{
		if (ID3D11Buffer* CbViewportRatio = DXDCB->GetConstantBuffer())
		{
			DXDDeviceContext->VSSetConstantBuffers(3, 1, &CbViewportRatio);
		}
	}
	if (UDXDConstantBuffer* DXDCB = ConstantBuffers[EConstantBufferType::Material])
	{
		if (ID3D11Buffer* CbMaterial = DXDCB->GetConstantBuffer())
		{
			DXDDeviceContext->PSSetConstantBuffers(4, 1, &CbMaterial);
		}
	}
	if (UDXDConstantBuffer* DXDCB = ConstantBuffers[EConstantBufferType::Gizmo])
	{
		if (ID3D11Buffer* CbGizmo = DXDCB->GetConstantBuffer())
		{
			DXDDeviceContext->PSSetConstantBuffers(3, 1, &CbGizmo);
		}
	}


	FWindowInfo winInfo = UEngine::GetEngine().GetWindowInfo();

	BufferManager = new UDXDBufferManager();

	/**
	* TODO: 뎁스 스텐실 스테이트 생성 권장.
	*       생성하지 않는 경우 기본값이 적용되므로, 현재 프로젝트에서는 따로 생성하지 않아도 됨.
	*       기본값:
	*         깊이 테스트 = TRUE
	*         깊이 비교 함수 = LESS (깊이값이 더 작을 때만 그려짐. 즉 더 가까이 있는 경우)
	*         깊이 쓰기 = TRUE
	*         스텐실 테스트 = FALSE
	*/
    //DepthStencilState = new UDXDDepthStencilState();
    //hr = DepthStencilState->CreateDepthStencilState(DXDDevice);
    //if (FAILED(hr))
    //    return hr;

    // 뎁스 스텐실 스테이트 생성.
	// 기본.
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
	memset(&DepthStencilDesc, 0, sizeof(DepthStencilDesc));
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	AddDepthStencilState(TEXT("Default"), DepthStencilDesc);

	// 깊이 무시하는 DepthStencilState
	memset(&DepthStencilDesc, 0, sizeof(DepthStencilDesc));
	DepthStencilDesc.DepthEnable = FALSE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	AddDepthStencilState(TEXT("Always"), DepthStencilDesc);

	ResourceManager = new UDXDResourceManager(DXDDevice);

    
	// 텍스쳐 파일 저장.
	FString FileName = TEXT("Resource/Texture/Fonts/DejaVu_Sans_Mono.dds");

	hr = ResourceManager->CreateTextureSRV(FileName);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = -1.0f;

	SamplerDesc.MaxAnisotropy = 16;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = 0;
	hr = ResourceManager->CreateTextureSampler(FileName, SamplerDesc);
	if (FAILED(hr))
	{
		return hr;
	}

	DynamicVertexBufferSize = 1024;
	
	CreatePrimitives(this);

	if (FAILED(hr))
	{
		return hr;
	}

    return S_OK;
}

void UDirectXHandle::ReleaseDirectX11Handle()
{
	if (DXDDeviceContext)
	{
		DXDDeviceContext->Flush();
		DXDDeviceContext->Release();
		DXDDeviceContext = nullptr;
	}

	if (DXDDevice)
	{
		DXDDevice->Release();
		DXDDevice = nullptr;
	}

	if (DXDSwapChain)
	{
		DXDSwapChain->Release();
		DXDSwapChain = nullptr;
	}

	for (const auto& [String, RenderTarget] : RenderTargets) {
		RenderTarget->ReleaseRenderTarget();
	}

	for (const auto& [String, DepthStencilView] : DepthStencilViews) {
		DepthStencilView->ReleaseDepthStencilView();
	}

	for (const auto& [String, DepthStencilState] : DepthStencilStates) {
		DepthStencilState->ReleaseDepthStencilState();
	}

	if (!RasterizerStates.empty())
	{
		for (auto& Rasterizer : RasterizerStates)
		{
			Rasterizer.second->ReleaseRasterizerState();
			delete Rasterizer.second;
		}
		RasterizerStates.clear();
	}
	
	if (ShaderManager)
	{
		ShaderManager->ReleaseAllShader();
		delete ShaderManager;
		ShaderManager = nullptr;
	}
	
	if (BufferManager)
	{
		BufferManager->ReleaseBuffers();
		delete BufferManager;
		BufferManager = nullptr;
	}

	if (!VertexBuffers.empty())
	{
		for (auto& VertexBuffer : VertexBuffers)
		{
			if (VertexBuffer.second.VertexBuffer)
			{
				VertexBuffer.second.VertexBuffer->Release();
				VertexBuffer.second.VertexBuffer = nullptr;
			}
		}
		VertexBuffers.clear();
	}

	if (IndexBuffers.empty())
	{
		for (auto& IndexBuffer : IndexBuffers)
		{
			IndexBuffer.second.IndexBuffer->Release();
			IndexBuffer.second.IndexBuffer = nullptr;
		}
		VertexBuffers.clear();
	}
	
	if (!ConstantBuffers.empty())
	{
		for (auto& ConstantBuffer : ConstantBuffers)
		{
			ConstantBuffer.second->Release();
			delete ConstantBuffer.second;
			ConstantBuffer.second = nullptr;
		}
		ConstantBuffers.clear();
	}
}

void UDirectXHandle::UpdateCameraMatrix( FViewportCamera* Camera)
{
	// Camera->GetCameraComponent.
	// MVP 계산 행렬 구하고
	// 카메라 상수버퍼로 바로 전달.
	if (!Camera)
	{
		return;
	}

	// 카메라 View 변환.
	// 카메라 컴포넌트의 로컬 트랜스폼은 변경되지 않는다고 가정
	ID3D11Buffer* CbChangesEveryFrame = ConstantBuffers[EConstantBufferType::ChangesEveryFrame]->GetConstantBuffer();
	if (!CbChangesEveryFrame)
	{
		return;
	}
	D3D11_MAPPED_SUBRESOURCE viewMappedData;
	DXDDeviceContext->Map(CbChangesEveryFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &viewMappedData);
	if (FCbChangesEveryFrame* Buffer = reinterpret_cast<FCbChangesEveryFrame*>(viewMappedData.pData))
	{
		Buffer->ViewMatrix = Camera->CachedViewMatrix;
	}
	DXDDeviceContext->Unmap(CbChangesEveryFrame, 0);

    // 카메라 Projection 변환
    // TODO: Test. 프로젝션 matrix는 리사이즈 할 때, FOV 변환할 때.
    ID3D11Buffer* CbChangesOnResize = ConstantBuffers[EConstantBufferType::ChangesOnResize]->GetConstantBuffer();
    if (!CbChangesOnResize )
    {
        return;
    }
    D3D11_MAPPED_SUBRESOURCE projectionMappedData;
    DXDDeviceContext->Map(CbChangesOnResize, 0, D3D11_MAP_WRITE_DISCARD, 0, &projectionMappedData);
    if (FCbChangesOnResize* Buffer = reinterpret_cast<FCbChangesOnResize*>(projectionMappedData.pData))
    {
		Buffer->ProjectionMatrix = Camera->CachedProjectionMatrix;
	}
	DXDDeviceContext->Unmap(CbChangesOnResize, 0);
	//XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), Width / Height, 1.f, 1000.f));
}

// use when D3D11_PRIMITIVE_TOPOLOGY_LINELIST state
void UDirectXHandle::RenderWorldPlane(const FViewportCamera* Camera) {

	if (!Camera)
		return;
    /** state check
    D3D11_PRIMITIVE_TOPOLOGY topology;
    DXDDeviceContext->IAGetPrimitiveTopology(&topology);
    if ( topology != D3D11_PRIMITIVE_TOPOLOGY_LINELIST )
        DXDDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    */

	DXDDeviceContext->VSSetShader(ShaderManager->GetVertexShaderByKey(TEXT("DefaultVS")), NULL, 0);
	DXDDeviceContext->PSSetShader(ShaderManager->GetPixelShaderByKey(TEXT("DefaultPS")), NULL, 0);

	DXDDeviceContext->IASetInputLayout(ShaderManager->GetInputLayoutByKey(TEXT("DefaultVS")));

    // set position
	float s = 1.F;//Camera->GridScale;
    FVector campos = Camera->Location;
    FVector truncpos = FVector(floor(campos.X/s)*s, floor(campos.Y/s)*s, 0.f);

    ID3D11Buffer* CbChangesEveryObject = ConstantBuffers[EConstantBufferType::ChangesEveryObject]->GetConstantBuffer();
    if ( !CbChangesEveryObject ) {
        return;
    }
    D3D11_MAPPED_SUBRESOURCE MappedData = {};
    DXDDeviceContext->Map(CbChangesEveryObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
    if ( FCbChangesEveryObject* Buffer = reinterpret_cast<FCbChangesEveryObject*>(MappedData.pData) ) {
		
        Buffer->WorldMatrix = FMatrix::GetScaleMatrix(FVector(s, s, 0)) * FMatrix::GetTranslateMatrix(truncpos);
    }
    DXDDeviceContext->Unmap(CbChangesEveryObject, 0);

    uint Stride = sizeof(FVertexPNCT);
    uint offset = 0;
    FVertexInfo Info = VertexBuffers[GetPrimitiveTypeAsString(EPrimitiveType::Grid)];
    ID3D11Buffer* VB = Info.VertexBuffer;
    uint Num = Info.NumVertices;
    DXDDeviceContext->IASetVertexBuffers(0, 1, &VB, &Stride, &offset);
    DXDDeviceContext->Draw(Num, 0);

}

void UDirectXHandle::RenderAABB(FBoundingBox InBox) {
    // Begin Object Matrix Update. 
    ID3D11Buffer* CbChangesEveryObject = ConstantBuffers[EConstantBufferType::ChangesEveryObject]->GetConstantBuffer();
    if ( !CbChangesEveryObject ) {
        return;
    }
    D3D11_MAPPED_SUBRESOURCE MappedData = {};
    DXDDeviceContext->Map(CbChangesEveryObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
    if ( FCbChangesEveryObject* Buffer = reinterpret_cast<FCbChangesEveryObject*>(MappedData.pData) ) {
		FVector Scale, Location;

		InBox.GetCenterAndExtents(Location, Scale);
		Buffer->WorldMatrix = FMatrix::GetScaleMatrix(Scale*2) * FMatrix::GetTranslateMatrix(Location);
    }
    DXDDeviceContext->Unmap(CbChangesEveryObject, 0);

    uint Stride = sizeof(FVertexPC);
    UINT offset = 0;
    FVertexInfo VertexInfo = VertexBuffers[GetPrimitiveTypeAsString(EPrimitiveType::BoundingBox)];
    ID3D11Buffer* VB = VertexInfo.VertexBuffer;
    DXDDeviceContext->IASetVertexBuffers(0, 1, &VB, &Stride, &offset);

	FIndexInfo IndexInfo = IndexBuffers[GetPrimitiveTypeAsString(EPrimitiveType::BoundingBox)];
	ID3D11Buffer* IB = IndexInfo.IndexBuffer;
	DXDDeviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);
	DXDDeviceContext->DrawIndexed(IndexInfo.NumIndices, 0, 0);
}

void UDirectXHandle::InitWindow(HWND hWnd, UINT InWidth, UINT InHeight)
{
	// 버퍼 준비
	// viewport
	WindowViewport.TopLeftX = 0;
	WindowViewport.TopLeftY = 0;
	WindowViewport.Width = InWidth;
	WindowViewport.Height = InHeight;
	WindowViewport.MinDepth = 0.0f;
	WindowViewport.MaxDepth = 1.0f;

	// rendertarget
	// swapchain을 target framebuffer로 하도록 설정.
	AddRenderTargetToSwapChain(TEXT("Window"));

	// depth stencil view
	AddDepthStencilView(TEXT("Window"), InWidth, InHeight);

	// depth stencil state
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
	memset(&DepthStencilDesc, 0, sizeof(DepthStencilDesc));
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	AddDepthStencilState(TEXT("Window"), DepthStencilDesc);


	// Quad 준비
	// Sampler State 생성 
	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ResourceManager->CreateTextureSampler(TEXT("Quad"), SamplerDesc);

}

// Quad를 그릴 준비를 합니다.
void UDirectXHandle::PrepareWindow()
{
	FLOAT ClearColor[4] = { 0.f, 0.f, 0.f, 1.0f };

	DXDDeviceContext->ClearRenderTargetView(GetRenderTarget(TEXT("Window"))->GetFrameBufferRTV().Get(), ClearColor);

	DXDDeviceContext->ClearDepthStencilView(GetDepthStencilView(TEXT("Window"))->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	UINT Stride = sizeof(FVertexPT);
	UINT Offset = 0;

	DXDDeviceContext->IASetInputLayout(GetShaderManager()->GetInputLayoutByKey(TEXT("FontVS")));
	DXDDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffers[TEXT("Quad")].VertexBuffer, &Stride, &Offset);
	DXDDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 셰이더 설정
	DXDDeviceContext->VSSetShader(GetShaderManager()->GetVertexShaderByKey(TEXT("ViewportVS")), nullptr, 0);
	DXDDeviceContext->PSSetShader(GetShaderManager()->GetPixelShaderByKey(TEXT("ViewportPS")), nullptr, 0);

	DXDDeviceContext->RSSetViewports(1, &WindowViewport);
	DXDDeviceContext->RSSetState(RasterizerStates[TEXT("Default")]->GetRasterizerState());

	SetFaceMode();

	ID3D11SamplerState* sampler = ResourceManager->TryGetTextureSampler(TEXT("Quad"));
	DXDDeviceContext->PSSetSamplers(0, 1, &sampler);

	DXDDeviceContext->OMSetRenderTargets(1, GetRenderTarget(TEXT("Window"))->GetFrameBufferRTV().GetAddressOf(), GetDepthStencilView(TEXT("Window"))->GetDepthStencilView());
}

// back buffer를 present합니다.
void UDirectXHandle::RenderWindow()
{
	// texture resource 해제.
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	DXDDeviceContext->PSSetShaderResources(0, 1, nullSRV);

	DXDSwapChain->Present(1, 0);
}

// texture2d를 생성합니다.
void UDirectXHandle::PrepareViewport(const FViewport& InViewport)
{
	FLOAT ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	DXDDeviceContext->ClearRenderTargetView(GetRenderTarget(InViewport.GetName())->GetFrameBufferRTV().Get(), ClearColor);

	// 뎁스/스텐실 뷰 클리어. 뷰, DEPTH만 클리어, 깊이 버퍼 클리어 할 값, 스텐실 버퍼 클리어 할 값.
	DXDDeviceContext->ClearDepthStencilView(GetDepthStencilView(InViewport.GetName())->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//DXDDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// FViewport의 것을 쓰는게 아니라 render target에 꽉 차게 설정해야함.
	// 크기만 가져와서 rasterizer에서 사용하도록 만듦.
	D3D11_VIEWPORT Viewport = InViewport.GetViewport();
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;

	DXDDeviceContext->RSSetViewports(1, &Viewport);
	DXDDeviceContext->RSSetState(RasterizerStates[TEXT("Default")]->GetRasterizerState());
	/*if (UEngine::GetEngine().ViewModeIndex == EViewModeIndex::VMI_Wireframe)
		DXDDeviceContext->RSSetState(RasterizerStates[TEXT("Wireframe")]->GetRasterizerState());*/

	// TODO: SwapChain Window 크기와 DepthStencilView Window 크기가 맞아야 에러 X.
	DXDDeviceContext->OMSetRenderTargets(1, GetRenderTarget(InViewport.GetName())->GetFrameBufferRTV().GetAddressOf(), GetDepthStencilView(InViewport.GetName())->GetDepthStencilView());
	DXDDeviceContext->OMSetDepthStencilState(GetDepthStencilState(TEXT("Default"))->GetDepthStencilState(), 0);
}

// quad를 그립니다.
void UDirectXHandle::RenderViewport(FViewport& InViewport, bool isDepthStencil)
{	
	if (InViewport.bIsResized)
	{
		UpdateViewportBuffer(InViewport);
		InViewport.bIsResized = false;
	}

	if (!isDepthStencil)
	{
		DXDDeviceContext->PSSetShaderResources(0, 1, GetRenderTarget(InViewport.GetName())->GetFrameBufferSRV().GetAddressOf());
	}
	else
	{
		auto SRV = GetDepthStencilView(InViewport.GetName())->GetDepthStencilSRV();
		DXDDeviceContext->PSSetShaderResources(0, 1, &SRV);
	}


	// viewport 위치 지정.
	D3D11_VIEWPORT viewport = WindowViewport;
	FWindowInfo windowInfo = UEngine::GetEngine().GetWindowInfo();
	ID3D11Buffer* CbViewportRatio = ConstantBuffers[EConstantBufferType::ViewportRatio]->GetConstantBuffer();
	if (!CbViewportRatio)
	{
		return;
	}

	//DXDDeviceContext->VSSetConstantBuffers(3, 1, &CbViewportRatio);
	D3D11_MAPPED_SUBRESOURCE MappedData = {};
	auto hr = DXDDeviceContext->Map(CbViewportRatio, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
	if (FCbViewportRatio* Buffer = reinterpret_cast<FCbViewportRatio*>(MappedData.pData))
	{
		Buffer->x = InViewport.GetViewport().TopLeftX / (float)windowInfo.Width * 2 - 1;
		Buffer->y = InViewport.GetViewport().TopLeftY / (float)windowInfo.Height * -2 + 1;
		Buffer->width = InViewport.GetViewport().Width / (float)windowInfo.Width * 2;
		Buffer->height = InViewport.GetViewport().Height / (float)windowInfo.Height * 2;
	}
	DXDDeviceContext->Unmap(CbViewportRatio, 0);
	DXDDeviceContext->Draw(VertexBuffers[TEXT("Quad")].NumVertices, 0);
}

HRESULT UDirectXHandle::UpdateViewportBuffer(const FViewport& InViewport)
{
	FString Name = InViewport.GetName();
	ReleaseRenderTarget(Name);
	ReleaseDepthStencilView(Name);

	// 컬러 버퍼.
	D3D11_TEXTURE2D_DESC TextureDesc = {};
	TextureDesc.Width = InViewport.GetViewport().Width;
	TextureDesc.Height = InViewport.GetViewport().Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc = {};
	RenderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetViewDesc.Texture2D.MipSlice = 0;

	HRESULT hr = S_OK;

	hr = AddRenderTarget(Name, TextureDesc, RenderTargetViewDesc);
	if (FAILED(hr))
	{
		UE_LOG(LogTemp, Warning, TEXT("FViewport::ResizeViewport::Failed to add render target"));
		return hr;
	}
	hr = AddDepthStencilView(Name, TextureDesc.Width, TextureDesc.Height);
	if (FAILED(hr))
	{
		UE_LOG(LogTemp, Warning, TEXT("FViewport::ResizeViewport::Failed to add render target"));
		return hr;
	}
	return hr;

}

HRESULT UDirectXHandle::AddRenderTarget(const FString& InName, const D3D11_TEXTURE2D_DESC InRenderTargetDesc, const D3D11_RENDER_TARGET_VIEW_DESC& InRenderTargetViewDesc)
{
	if (RenderTargets.find(InName) != RenderTargets.end())
	{
		UE_LOG(LogTemp, Display, TEXT("UDirectXHandle::AddRenderTarget::Duplicate Name"));
		return S_OK;
	}

	UDXDRenderTarget* RenderTarget = new UDXDRenderTarget();

	HRESULT hr = RenderTarget->CreateRenderTarget(DXDDevice, InRenderTargetDesc ,InRenderTargetViewDesc);
	if (FAILED(hr))
		return hr;

	RenderTargets[InName] = RenderTarget;
	return S_OK;
}

HRESULT UDirectXHandle::AddRenderTargetToSwapChain(const FString& InName)
{
	if (RenderTargets.find(InName) != RenderTargets.end())
	{
		UE_LOG(LogTemp, Display, TEXT("UDirectXHandle::AddRenderTarget::Duplicate Name"));
		return S_OK;
	}

	UDXDRenderTarget* RenderTarget = new UDXDRenderTarget();

	HRESULT hr = RenderTarget->CreateRenderTargetToSwapChain(DXDDevice, DXDSwapChain);
	if (FAILED(hr))
		return hr;

	RenderTargets[InName] = RenderTarget;
	return S_OK;
}

HRESULT UDirectXHandle::AddDepthStencilView(const FString& InName, UINT InWidth, UINT InHeight)
{
	if (DepthStencilViews.find(InName) != DepthStencilViews.end())
	{
		UE_LOG(LogTemp, Display, TEXT("UDirectXHandle::AddDepthStencilView::Duplicate Name"));
		return S_OK;
	}

	UDXDDepthStencilView* DepthStencilView = new UDXDDepthStencilView();

	HRESULT hr = DepthStencilView->CreateDepthStencilView(DXDDevice, InWidth, InHeight);
	if (FAILED(hr))
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::AddDepthStencilView::Create Failed"));
		return hr;
	}

	DepthStencilViews[InName] = DepthStencilView;
	return S_OK;
}

HRESULT UDirectXHandle::AddDepthStencilState(const FString& InName, const D3D11_DEPTH_STENCIL_DESC& InDesc)
{
	if (DepthStencilStates.find(InName) != DepthStencilStates.end())
	{
		UE_LOG(LogTemp, Display, TEXT("UDirectXHandle::AddDepthStencilState::Duplicate Name"));
		return S_OK;
	}

	UDXDDepthStencilState* DepthStencilState = new UDXDDepthStencilState();

	HRESULT hr = DepthStencilState->CreateDepthStencilState(DXDDevice, InDesc);
	if (FAILED(hr))
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::AddDepthStencilState::Create Failed"));
		return hr;
	}

	DepthStencilStates[InName] = DepthStencilState;
	return S_OK;
}

HRESULT UDirectXHandle::AddRasterizerState(const FString& InName, const D3D11_RASTERIZER_DESC& InDesc)
{
	if (RasterizerStates.find(InName) != RasterizerStates.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::AddDepthStencilView::Duplicate Name"));
		return S_OK;
	}

	UDXDRasterizerState* RasterizerState = new UDXDRasterizerState();

	HRESULT hr = RasterizerState->CreateRasterizerState(DXDDevice, InDesc);
	if (FAILED(hr))
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::AddRasterizerState::Create Failed"));
		return hr;
	}

	RasterizerStates[InName] = RasterizerState;
	return S_OK;
}

UDXDRenderTarget* UDirectXHandle::GetRenderTarget(const FString& InName)
{
	if (RenderTargets.find(InName) == RenderTargets.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::GetRenderTarget::Invalid Name"));
		return nullptr;
	}
	return RenderTargets[InName];
}

UDXDDepthStencilView* UDirectXHandle::GetDepthStencilView(const FString& InName)
{
	if (DepthStencilViews.find(InName) == DepthStencilViews.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::GetDepthStencilView::Invalid Name"));
		return nullptr;
	}
	return DepthStencilViews[InName];
}

UDXDDepthStencilState* UDirectXHandle::GetDepthStencilState(const FString& InName)
{
	if (DepthStencilStates.find(InName) == DepthStencilStates.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::GetDepthStencilState::Invalid Name"));
		return nullptr;
	}
	return DepthStencilStates[InName];
}

UDXDRasterizerState* UDirectXHandle::GetRasterizerState(const FString& InName)
{
	if (RasterizerStates.find(InName) == RasterizerStates.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::GetRasterizerState::Invalid Name"));
		return nullptr;
	}
	return RasterizerStates[InName];
}

HRESULT UDirectXHandle::ReleaseRenderTarget(const FString& InName)
{
	if (RenderTargets.find(InName) == RenderTargets.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::ReleaseRenderTarget::Invalid Name"));
		return S_FALSE;
	}

	RenderTargets[InName]->ReleaseRenderTarget();
	RenderTargets.erase(InName);

	return S_OK;
}

HRESULT UDirectXHandle::ReleaseDepthStencilView(const FString& InName)
{
	if (DepthStencilViews.find(InName) == DepthStencilViews.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::ReleaseDepthStencilView::Invalid Name"));
		return S_FALSE;
	}

	DepthStencilViews[InName]->ReleaseDepthStencilView();
	DepthStencilViews.erase(InName);

	return S_OK;
}

void UDirectXHandle::RenderDebugRays(const TArray<FRay>& Rays)
{
	FVertexInfo v = GetVertexBuffer(TEXT("Line"));

	DXDDeviceContext->VSSetShader(ShaderManager->GetVertexShaderByKey(TEXT("LineVS")), NULL, 0);
	DXDDeviceContext->PSSetShader(ShaderManager->GetPixelShaderByKey(TEXT("LinePS")), NULL, 0);

	DXDDeviceContext->IASetInputLayout(ShaderManager->GetInputLayoutByKey(TEXT("LineVS")));

	D3D11_PRIMITIVE_TOPOLOGY top;
	DXDDeviceContext->IAGetPrimitiveTopology(&top);
	SetLineMode();

	for (auto& ray : Rays)
	{
		// Begin Object Matrix Update. 
		ID3D11Buffer* CbChangesEveryObject = ConstantBuffers[EConstantBufferType::ChangesEveryObject]->GetConstantBuffer();
		if (!CbChangesEveryObject)
		{
			return;
		}
		D3D11_MAPPED_SUBRESOURCE MappedData = {};
		DXDDeviceContext->Map(CbChangesEveryObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
		if (FCbChangesEveryObject* Buffer = reinterpret_cast<FCbChangesEveryObject*>(MappedData.pData))
		{
			Buffer->WorldMatrix = FMatrix(
				FVector4(ray.Direction.X, ray.Direction.Y, ray.Direction.Z, 0),
				//FVector4(1, 0, 0, 0),
				FVector4(0, 1, 0, 0),
				FVector4(0, 0, 1, 0),
				FVector4(ray.Origin.X, ray.Origin.Y, ray.Origin.Z, 1)
			);
			//FMatrix Rot = FMatrix::MakeFromDirection(ray.Direction, FVector(0, 0, 1));
			//Buffer->WorldMatrix = Rot * Buffer->WorldMatrix;
		}
		DXDDeviceContext->Unmap(CbChangesEveryObject, 0);

		uint Stride = sizeof(FVertexPC);
		UINT offset = 0;

		DXDDeviceContext->IASetVertexBuffers(0, 1, &v.VertexBuffer, &Stride, &offset);
		DXDDeviceContext->Draw(v.NumVertices, 0);

	}
	DXDDeviceContext->IASetPrimitiveTopology(top);

}


void UDirectXHandle::RenderBoundingBox(const TArray<AActor*> Actors) {

	DXDDeviceContext->VSSetShader(ShaderManager->GetVertexShaderByKey(TEXT("LineVS")), NULL, 0);
	DXDDeviceContext->PSSetShader(ShaderManager->GetPixelShaderByKey(TEXT("LinePS")), NULL, 0);

	DXDDeviceContext->IASetInputLayout(ShaderManager->GetInputLayoutByKey(TEXT("LineVS")));

	SetLineMode();

	for (TObjectIterator<AActor> It; It; ++It)
	{
		AActor* Actor = *It;
		RenderAABB(Actor->GetAABB());
	}

}

void UDirectXHandle::RenderGizmo(AGizmoActor* Gizmos) {

	SetFaceMode();
	DXDDeviceContext->VSSetShader(ShaderManager->GetVertexShaderByKey(TEXT("GizmoVS")), NULL, 0);
	DXDDeviceContext->PSSetShader(ShaderManager->GetPixelShaderByKey(TEXT("GizmoPS")), NULL, 0);

	DXDDeviceContext->IASetInputLayout(ShaderManager->GetInputLayoutByKey(TEXT("GizmoVS")));

    if (!Gizmos)
        return;

	
    // Begin Object Matrix Update. 
    ID3D11Buffer* CbChangesEveryObject = ConstantBuffers[EConstantBufferType::ChangesEveryObject]->GetConstantBuffer();
    if (!CbChangesEveryObject)
    {
        return;
    } 
    D3D11_MAPPED_SUBRESOURCE MappedData = {};
    DXDDeviceContext->Map(CbChangesEveryObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
    if (FCbChangesEveryObject* Buffer = reinterpret_cast<FCbChangesEveryObject*>(MappedData.pData))
    {
		Buffer->WorldMatrix = FMatrix::Identity();
    }
    DXDDeviceContext->Unmap(CbChangesEveryObject, 0);

	ID3D11Buffer* Gizmo = ConstantBuffers[EConstantBufferType::Gizmo]->GetConstantBuffer();
	if (!Gizmo)
	{
		return;
	}
	MappedData = {};
	DXDDeviceContext->Map(Gizmo, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
	if (FCbGizmo* Buffer = reinterpret_cast<FCbGizmo*>(MappedData.pData))
	{
		Buffer->Color = FVector4(1, 0, 0, 1);
	}
	DXDDeviceContext->Unmap(Gizmo, 0);


    DXDDeviceContext->OMSetDepthStencilState(GetDepthStencilState(TEXT("Always"))->GetDepthStencilState(), 0);

	FStaticMesh* MeshInfo = FObjManager::LoadObjStaticMeshAsset(Gizmos->GetGizmoTranslateComponent()->GetStaticMesh()->GetAssetPathFileName());

	uint Stride = sizeof(FVertexPNCT);
	UINT offset = 0;

	FVertexInfo VertexInfo;
	FIndexInfo IndexInfo;
	BufferManager->CreateVertexBuffer(DXDDevice, MeshInfo->Sections[0].Vertices, VertexInfo);
	BufferManager->CreateIndexBuffer(DXDDevice, MeshInfo->Sections[0].Indices, IndexInfo);

	// Vertex 버퍼 바인딩
	DXDDeviceContext->IASetVertexBuffers(0, 1, &VertexInfo.VertexBuffer, &Stride, &offset);

	// Index 버퍼 바인딩
	DXDDeviceContext->IASetIndexBuffer(IndexInfo.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 인덱스를 기반으로 그리기
	DXDDeviceContext->DrawIndexed(IndexInfo.NumIndices, 0, 0);

	DXDDeviceContext->OMSetDepthStencilState(GetDepthStencilState(TEXT("Default"))->GetDepthStencilState(), 0);
}

void UDirectXHandle::RenderStaticMesh(UStaticMeshComponent* Comp)
{
	if (!Comp)
		return;

	DXDDeviceContext->VSSetShader(ShaderManager->GetVertexShaderByKey(TEXT("DefaultVS")), NULL, 0);
	DXDDeviceContext->PSSetShader(ShaderManager->GetPixelShaderByKey(TEXT("DefaultPS")), NULL, 0);
	DXDDeviceContext->IASetInputLayout(ShaderManager->GetInputLayoutByKey(TEXT("DefaultVS")));
	
	// Begin Object Matrix Update. 
	ID3D11Buffer* CbChangesEveryObject = ConstantBuffers[EConstantBufferType::ChangesEveryObject]->GetConstantBuffer();
	if (!CbChangesEveryObject)
	{
		return;
	}
	D3D11_MAPPED_SUBRESOURCE MappedData = {};
	DXDDeviceContext->Map(CbChangesEveryObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
	if (FCbChangesEveryObject* Buffer = reinterpret_cast<FCbChangesEveryObject*>(MappedData.pData))
	{
		Buffer->WorldMatrix = Comp->GetWorldMatrix();
	}
	DXDDeviceContext->Unmap(CbChangesEveryObject, 0);

	uint Stride = sizeof(FVertexPNCT);
	UINT offset = 0;

	UStaticMesh* Mesh = Comp->GetStaticMesh();
	
	FStaticMesh* MeshInfo = FObjManager::LoadObjStaticMeshAsset(Comp->GetStaticMesh()->GetAssetPathFileName());


	// 각 섹션별로 처리
	for (const FStaticMeshSection& Section : MeshInfo->Sections)
	{
		
		UMaterial* Mat = FMaterialManager::LoadMaterial(Section.MaterialName);
		
		ID3D11ShaderResourceView* DiffuseSRV = nullptr;

		if (Mat)
		{
			UTexture* AmbientTextureMap = Mat->GetAmbientTextureMap();
			UTexture* DiffuseTextureMap = Mat->GetDiffuseTextureMap();
			UTexture* SpecularTextureMap = Mat->GetSpecularTextureMap();
			UTexture* EmissiveTextureMap = Mat->GetEmissiveTextureMap();
			UTexture* BumpMap = Mat->GetBumpMap();
			UTexture* DisplacementMap = Mat->GetDisplacementMap();

			ID3D11Buffer* CbMaterial = ConstantBuffers[EConstantBufferType::Material]->GetConstantBuffer();
			if (!CbChangesEveryObject)
			{
				return;
			}
			D3D11_MAPPED_SUBRESOURCE MappedData = {};
			DXDDeviceContext->Map(CbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
			if (FCbMaterial* Buffer = reinterpret_cast<FCbMaterial*>(MappedData.pData))
			{
				Buffer->Ambient = Mat->GetAmbient();
				Buffer->Diffuse = Mat->GetDiffuse();
				Buffer->Specular = Mat->GetSpecular();
				Buffer->Emissive = Mat->GetEmissive();
				Buffer->SpecularExponent = Mat->GetSpecularExponent();
				Buffer->IlluminationModel = Mat->GetIlluminationModel();
				Buffer->Opacity = Mat->GetOpacity();
				Buffer->Transparency = Mat->GetTransparency();
				Buffer->RefractiveIndex = Mat->GetRefractiveIndex();
			}
			DXDDeviceContext->Unmap(CbMaterial, 0);

			// TODO: 모든 텍스쳐 적용할 수 있도록 잡기.
			/*ID3D11ShaderResourceView* textures[] =
			{
				ResourceManager->TryGetTextureSRV(AmbientTextureMap->GetTextureName()),
				ResourceManager->TryGetTextureSRV(DiffuseTextureMap->GetTextureName()),
				ResourceManager->TryGetTextureSRV(SpecularTextureMap->GetTextureName()),
				ResourceManager->TryGetTextureSRV(EmissiveTextureMap->GetTextureName()),
				ResourceManager->TryGetTextureSRV(BumpMap->GetTextureName()),
				ResourceManager->TryGetTextureSRV(DisplacementMap->GetTextureName()),
			};*/
			if (DiffuseTextureMap)
			{
				DiffuseSRV = ResourceManager->TryGetTextureSRV(DiffuseTextureMap->GetTextureName());
			}


		}
		
		DXDDeviceContext->PSSetShaderResources(1, 1, &DiffuseSRV);
		ID3D11SamplerState* Sampler = ResourceManager->TryGetTextureSampler(TEXT("Resource/Texture/Fonts/DejaVu_Sans_Mono.dds"));
		DXDDeviceContext->PSSetSamplers(0, 1, &Sampler);

		// Vertex/Index 버퍼 생성
		FVertexInfo VertexInfo;
		FIndexInfo IndexInfo;
		BufferManager->CreateVertexBuffer(DXDDevice, Section.Vertices, VertexInfo);
		BufferManager->CreateIndexBuffer(DXDDevice, Section.Indices, IndexInfo);

		// Vertex 버퍼 바인딩
		DXDDeviceContext->IASetVertexBuffers(0, 1, &VertexInfo.VertexBuffer, &Stride, &offset);

		// Index 버퍼 바인딩
		DXDDeviceContext->IASetIndexBuffer(IndexInfo.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// 인덱스를 기반으로 그리기
		DXDDeviceContext->DrawIndexed(IndexInfo.NumIndices, 0, 0);
		
		VertexInfo.VertexBuffer->Release();
		IndexInfo.IndexBuffer->Release();
	}
}

void UDirectXHandle::RenderObject(const TArray<AActor*> Actors)
{
	/*if ( !GetFlag(UEngine::GetEngine().ShowFlags, EEngineShowFlags::SF_Primitives) )
		return;*/

    for (AActor* Actor : Actors)
    {
		for (UActorComponent* Comp : Actor->GetOwnedComponent())
		{
			RenderStaticMesh(Cast<UStaticMeshComponent>(Comp));
        } 
		RenderActorUUID(Actor);

		// 액터가 가진 모든 컴포넌트 순회하면서 렌더링.
		//RenderPrimitive(Actor->GetComponentByClass<UPrimitiveComponent>());
		// PrimitiveComponent가 없으면 그릴 게 없으므로 Pass;
	}


    // 셰이더 준비.
    // 현재 액터가 가진 Component 타입 별로 분석해서 셰이더 적용.
    // 컴포넌트에서 정보 가져와서 Constant 버퍼 업데이트.
    // 액터에 해당하는 오브젝트 렌더링.
}

void UDirectXHandle::RenderLines(const TArray<AActor*> Actors)
{
	
}

void UDirectXHandle::RenderActorUUID(AActor* TargetActor)
{
	//if ( !GetFlag(UEngine::GetEngine().ShowFlags, EEngineShowFlags::SF_BillboardText) )
		//return;

    if (!TargetActor)
        return;

	DXDDeviceContext->VSSetShader(ShaderManager->GetVertexShaderByKey(TEXT("FontVS")), NULL, 0);
	DXDDeviceContext->PSSetShader(ShaderManager->GetPixelShaderByKey(TEXT("FontPS")), NULL, 0);
	DXDDeviceContext->IASetInputLayout(ShaderManager->GetInputLayoutByKey(TEXT("FontVS")));
	// 깊이 테스트 무시하는 DepthStencilState로 변경.
	DXDDeviceContext->OMSetDepthStencilState(GetDepthStencilState(TEXT("Always"))->GetDepthStencilState(), 0);

    // Begin Object Matrix Update. 
    ID3D11Buffer* CbChangesEveryObject = ConstantBuffers[EConstantBufferType::ChangesEveryObject]->GetConstantBuffer();
    if (!CbChangesEveryObject)
    {
        return;
    }
    D3D11_MAPPED_SUBRESOURCE MappedData = {};
    DXDDeviceContext->Map(CbChangesEveryObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData);
    if (FCbChangesEveryObject* Buffer = reinterpret_cast<FCbChangesEveryObject*>(MappedData.pData))
    {
        // 프리미티브 위치에서 카메라 쪽으로 회전.
		/*ACamera* Camera = UEngine::GetEngine().GetWorld()->GetCamera();
		if (!Camera)
			return;
		FVector CameraLocation = Camera->GetActorLocation();
		FVector ActorLocation = TargetActor->GetActorLocation();
		FVector Delta = (CameraLocation - ActorLocation).GetSafeNormal();

		float Pitch = FMath::RadiansToDegrees(FMath::Asin(Delta.Z));
		float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Delta.Y, Delta.X));

		FRotator PrimitiveRotation(Pitch, Yaw, 0.f);
        FMatrix RotationMatrix(PrimitiveRotation);
        FMatrix TranslationMatrix = FMatrix::GetTranslateMatrix(ActorLocation);

        Buffer->WorldMatrix = RotationMatrix * TranslationMatrix;*/
    }
    DXDDeviceContext->Unmap(CbChangesEveryObject, 0);

	ID3D11ShaderResourceView* FontAtlasTexture = ResourceManager->TryGetTextureSRV(TEXT("Resource/Texture/Fonts/DejaVu_Sans_Mono.dds"));
	ID3D11SamplerState* FontSamplerState = ResourceManager->TryGetTextureSampler(TEXT("Resource/Texture/Fonts/DejaVu_Sans_Mono.dds"));
	DXDDeviceContext->PSSetShaderResources(0, 1, &FontAtlasTexture);
	DXDDeviceContext->PSSetSamplers(0, 1, &FontSamplerState);

    uint Stride = sizeof(FVertexPT);
    UINT offset = 0;
	FBufferInfo Info;
	BufferManager->CreateASCIITextBuffer(DXDDevice, TargetActor->GetName(), Info, 0.0f, 0.0f);
    DXDDeviceContext->IASetVertexBuffers(0, 1, &Info.VertexInfo.VertexBuffer, &Stride, &offset);
	DXDDeviceContext->IASetIndexBuffer(Info.IndexInfo.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DXDDeviceContext->DrawIndexed(Info.IndexInfo.NumIndices, 0, 0);

	// DepthStencilState 기본으로 변경
	DXDDeviceContext->OMSetDepthStencilState(GetDepthStencilState(TEXT("Default"))->GetDepthStencilState(), 0);


	Info.VertexInfo.VertexBuffer->Release();
	Info.IndexInfo.IndexBuffer->Release();
}

//void UDirectXHandle::InitView()
//{
//	// 렌더 타겟 클리어 및 클리어에 적용할 색.
//	FLOAT ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
//
//	DXDDeviceContext->ClearRenderTargetView(RenderTargets[TEXT("Default")]->GetFrameBufferRTV().Get(), ClearColor);
//
//	// 뎁스/스텐실 뷰 클리어. 뷰, DEPTH만 클리어, 깊이 버퍼 클리어 할 값, 스텐실 버퍼 클리어 할 값.
//	DXDDeviceContext->ClearDepthStencilView(DepthStencilViews[TEXT("Default")]->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
//
//    //DXDDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	DXDDeviceContext->RSSetViewports(1, &ViewportInfo);
//	if (UEngine::GetEngine().ViewModeIndex == EViewModeIndex::VMI_Wireframe)
//		DXDDeviceContext->RSSetState(RasterizerStates[TEXT("Wireframe")]->GetRasterizerState());
//	else
//		DXDDeviceContext->RSSetState(RasterizerStates[TEXT("Default")]->GetRasterizerState());
//
//	// TODO: SwapChain Window 크기와 DepthStencilView Window 크기가 맞아야 에러 X.
//	DXDDeviceContext->OMSetRenderTargets(1, RenderTargets[TEXT("Default")]->GetFrameBufferRTV().GetAddressOf(), DepthStencilViews[TEXT("Default")]->GetDepthStencilView());
//}

FVertexInfo UDirectXHandle::GetVertexBuffer(FString KeyName)
{
	if (VertexBuffers.find(KeyName) == VertexBuffers.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("UDirectXHandle::GetVertexBuffer::Invalid Name"));
		return FVertexInfo();
	}
	else
	{
		return VertexBuffers[KeyName];
	}
}

HRESULT UDirectXHandle::AddConstantBuffer(EConstantBufferType Type)
{
	UDXDConstantBuffer* NewConstantBuffer = new UDXDConstantBuffer(DXDDevice);
	if (!NewConstantBuffer)
	{
		return S_FALSE;
	}

	HRESULT hr = S_OK;

	hr = NewConstantBuffer->CreateBuffer(Type);
	if (FAILED(hr))
	{
		return hr;
	}

	ConstantBuffers.insert({ Type, NewConstantBuffer });

	return S_OK;
}


void UDirectXHandle::ResizeViewport(int width, int height) {
	WindowViewport.Width = static_cast<float>(width);
	WindowViewport.Height = static_cast<float>(height);
}

HRESULT UDirectXHandle::ResizeWindow(int width, int height) {

	//assert(0); // addrendertarget수정중
	RenderTargets[TEXT("Window")]->~UDXDRenderTarget();
	DepthStencilViews[TEXT("Window")]->~UDXDDepthStencilView();
	
	HRESULT hr = DXDSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
	if ( FAILED(hr) )
		return hr;

	D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
	framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
	framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처
	hr = RenderTargets[TEXT("Window")]->CreateRenderTargetToSwapChain(DXDDevice, DXDSwapChain);
	//hr = AddRenderTargetToSwapChain(TEXT("Window"));
	if ( FAILED(hr) )
		return hr;

	FWindowInfo winInfo = UEngine::GetEngine().GetWindowInfo();
	hr = DepthStencilViews[TEXT("Window")]->CreateDepthStencilView(DXDDevice, static_cast<float>(width), static_cast<float>(height));
	if ( FAILED(hr) )
		return hr;

	return hr;
}

