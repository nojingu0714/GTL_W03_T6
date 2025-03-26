#include "pch.h"

//#include "PrimitiveVertices.h"
#include "Core/Resource/Types.h"
#include "DirectX11/DXDBufferManager.h"

// 가장 기본적인 Primitive들을 생성하는 함수.
HRESULT CreatePrimitives(UDXDBufferManager* BufferManager)
{
	const int NumSegments = 32;
	HRESULT hr = S_OK;
	///////////////////////////////////
	// Line
	TArray<FVertexPC> LineVertices;
	LineVertices.push_back({ {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} });
	LineVertices.push_back({ {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} });

	hr = BufferManager->CreateVertexBuffer(TEXT("Line"), LineVertices, nullptr);

	///////////////////////////////////
	// Cube
	TArray<FVertexPC> CubeVertices;
	CubeVertices.push_back({ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }); // Bottom-left (red)
	CubeVertices.push_back({ {-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }); // Top-left (yellow)
	CubeVertices.push_back({ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }); // Bottom-right (green)
	CubeVertices.push_back({ {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} }); // Top-right (blue)

	// Back face (Z-)
	CubeVertices.push_back({ {-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} }); // Bottom-left (cyan)
	CubeVertices.push_back({ {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }); // Bottom-right (magenta)
	CubeVertices.push_back({ {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} }); // Top-left (blue)
	CubeVertices.push_back({ {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }); // Top-right (yellow)

	// Left face (X-)
	CubeVertices.push_back({ {-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} }); // Bottom-left (purple)
	CubeVertices.push_back({ {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} }); // Top-left (blue)
	CubeVertices.push_back({ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }); // Bottom-right (green)
	CubeVertices.push_back({ {-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} }); // Top-right (yellow)

	// Right face (X+)
	CubeVertices.push_back({ {0.5f, -0.5f, 0.5f}, {1.0f, 0.5f, 0.0f, 1.0f} }); // Bottom-left (orange)
	CubeVertices.push_back({ {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f} }); // Bottom-right (gray)
	CubeVertices.push_back({ {0.5f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f, 1.0f} }); // Top-left (purple)
	CubeVertices.push_back({ {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.5f, 1.0f} }); // Top-right (dark blue)

	// Top face (Y+)
	CubeVertices.push_back({ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.5f, 1.0f} }); // Bottom-left (light green)
	CubeVertices.push_back({ {-0.5f, 0.5f, 0.5f}, {0.0f, 0.5f, 1.0f, 1.0f} }); // Top-left (cyan)
	CubeVertices.push_back({ {0.5f, 0.5f, -0.5f}, {0.5f, 1.0f, 1.0f, 1.0f} }); // Bottom-right (white)
	CubeVertices.push_back({ {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 1.0f} }); // Top-right (brown)

	// Bottom face (Y-)
	CubeVertices.push_back({ {-0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 1.0f} }); // Bottom-left (brown)
	CubeVertices.push_back({ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }); // Top-left (red)
	CubeVertices.push_back({ {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.5f, 1.0f} }); // Bottom-right (purple)
	CubeVertices.push_back({ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }); // Top-right (green)

	TArray<uint32> CubeIndices = {
		// Front face
		0, 1, 2, 1, 3, 2,
		// Back face
		4, 5, 6, 6, 5, 7,
		// Left face
		8, 9, 10, 9, 11, 10,
		// Right face
		12, 13, 14, 14, 13, 15,
		// Top face
		16, 17, 18, 17, 19, 18,
		// Bottom face
		20, 21, 22, 21, 23, 22
	};

	hr = BufferManager->CreateVertexBuffer(TEXT("Cube"), CubeVertices, CubeIndices);
	hr = BufferManager->CreateIndexBuffer(TEXT("Cube"), CubeIndices, )

	///////////////////////////////////
	/// Cylinder
	TArray<FVertexPC> CylinderVertices;

	CylinderVertices.push_back({ {0.0f, 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }); // Bottom
	CylinderVertices.push_back({ {0.0f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }); // Top
	for (int i = 0; i <= NumSegments; i++)
	{
		float angle = 2.0f * PI * i / (float)NumSegments;
		float x = cos(angle);
		float y = sin(angle);
		CylinderVertices.push_back({ {x, y, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }); // Bottom
		CylinderVertices.push_back({ {x, y, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }); // Top
	}

	TArray<uint32> CylinderIndices;
	for (int i = 0; i < NumSegments; i++)
	{
		CylinderIndices.push_back(0);
		CylinderIndices.push_back(i * 2 + 2);
		CylinderIndices.push_back(i * 2 + 4);
		CylinderIndices.push_back(1);
		CylinderIndices.push_back(i * 2 + 5);
		CylinderIndices.push_back(i * 2 + 3);
		CylinderIndices.push_back(i * 2 + 2);
		CylinderIndices.push_back(i * 2 + 3);
		CylinderIndices.push_back(i * 2 + 4);
		CylinderIndices.push_back(i * 2 + 5);
		CylinderIndices.push_back(i * 2 + 4);
		CylinderIndices.push_back(i * 2 + 3);
	}

	hr = BufferManager->CreateVertexBuffer(TEXT("Cylinder"), CylinderVertices, CylinderIndices);

	///////////////////////////////////
	/// Cone

	TArray<FVertexPC> ConeVertices;
	ConeVertices.push_back({ {0.0f, 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }); // Bottom
	ConeVertices.push_back({ {0.0f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} }); // Top
	for (int i = 0; i <= NumSegments; i++)
	{
		float angle = 2.0f * PI * i / (float)NumSegments;
		float x = cos(angle);
		float y = sin(angle);
		ConeVertices.push_back({ {x, y, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} }); // Bottom
	}
	TArray<uint32> ConeIndices;
	for (int i = 0; i < NumSegments; i++)
	{
		ConeIndices.push_back(0);
		ConeIndices.push_back(i + 2);
		ConeIndices.push_back(i + 3);
		ConeIndices.push_back(1);
		ConeIndices.push_back(i + 3);
		ConeIndices.push_back(i + 2);
		ConeIndices.push_back(i + 2);
		ConeIndices.push_back(i + 3);
		ConeIndices.push_back(NumSegments + 2);
	}

	hr = BufferManager->CreateVertexBuffer(TEXT("Cone"), ConeVertices, ConeIndices);

	///////////////////////////////////
	///	Quarter Ring

	TArray<FVertexPC> QuarterRingVertices;
	for (int i = 0; i <= NumSegments / 4; i++)
	{
		float angle = 2.0f * PI * i / (float)NumSegments;
		float x = cos(angle);
		float y = sin(angle);
		QuarterRingVertices.push_back({ {x, y, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} });
		QuarterRingVertices.push_back({ {x, y, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} });
	}
	TArray<uint32> QuarterRingIndices;
	for (int i = 0; i < NumSegments / 4; i++)
	{
		QuarterRingIndices.push_back(i * 2);
		QuarterRingIndices.push_back(i * 2 + 1);
		QuarterRingIndices.push_back(i * 2 + 3);
		QuarterRingIndices.push_back(i * 2);
		QuarterRingIndices.push_back(i * 2 + 3);
		QuarterRingIndices.push_back(i * 2 + 2);
	}

	hr = Handle->CreateVertexBuffer(TEXT("QuarterRing"), QuarterRingVertices, QuarterRingIndices);

	///////////////////////////////////
	/// CubeFrame

	TArray<FVertexPC> CubeFrameVertices;
	CubeFrameVertices.push_back({ {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 0
	CubeFrameVertices.push_back({ {-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 1
	CubeFrameVertices.push_back({ {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 2
	CubeFrameVertices.push_back({ {0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 3
	CubeFrameVertices.push_back({ {-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 4
	CubeFrameVertices.push_back({ {0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 5
	CubeFrameVertices.push_back({ {-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 6
	CubeFrameVertices.push_back({ {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} }); // 7

	TArray<uint32> CubeFrameIndices = {
		// Bottom face
		0, 1, 1, 3, 3, 2, 2, 0,
		// Top face
		4, 6, 6, 7, 7, 5, 5, 4,
		// Side faces
		0, 4, 1, 6, 2, 5, 3, 7
	};

	hr = Handle->CreateVertexBuffer(TEXT("BoundingBox"), CubeFrameVertices, CubeFrameIndices);

	///////////////////////////////////
	/// Quad
	
	TArray<FVertexPT> QuadVertices =
	{
		{ FVector(1.f, -1.f, 0.f), FVector2(1.0f, 1.0f) }, // 우하단.
		{ FVector(0.f, 0.f, 0.f), FVector2(0.0f, 0.0f) }, // 좌상단.
		{ FVector(1.f, 0.f, 0.f), FVector2(1.0f, 0.0f) }, // 우상단.
		{ FVector(1.f, -1.f, 0.f), FVector2(1.0f, 1.0f) }, // 우하단.
		{ FVector(0.f, -1.f, 0.f), FVector2(0.0f, 1.0f) }, // 좌하단.
		{ FVector(0.f, 0.f, 0.f), FVector2(0.0f, 0.0f) }, // 좌상단.
	};
    // Change the line causing the error to explicitly specify the template parameter
	TArray<uint32> EmptyIndices;

    hr = Handle->CreateVertexBuffer(TEXT("Quad"), QuadVertices, EmptyIndices);
	return hr;


}