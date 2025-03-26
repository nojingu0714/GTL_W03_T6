#include "pch.h"
#include "Splitter.h"
#include "Viewport.h"
#include "Engine/Engine.h"

void FSplitter::Init(FViewport* InTopLeft, FViewport* InTopRight, FViewport* InBottomLeft, FViewport* InBottomRight)
{
	{
		TopLeft = InTopLeft;
		TopRight = InTopRight;
		BottomLeft = InBottomLeft;
		BottomRight = InBottomRight;

		MinViewportWidth = 100;
		MinViewportHeight = 100;

		bIsDragging = false;

		CalculateInitialSplitterPosition();
		SplitterThickness = 10.f;
		UpdateSplitterRatio();
	}
}

void FSplitter::SetSplitterPosition(FVector2 Position)
{
	SplitterPosition = Position;
	UpdateSplitterRatio();
}

FVector2 FSplitter::GetPosition() const
{
	return SplitterPosition;
}

void FSplitter::UpdateSplitterRatio()
{
	/*float TotalWidth = TopLeft->GetViewport().Width + TopRight->GetViewport().Width;
	float TotalHeight = TopLeft->GetViewport().Height + BottomLeft->GetViewport().Height;*/

	SplitterRatio.X = SplitterPosition.X / UEngine::GetEngine().GetWindowInfo().Width;
	SplitterRatio.Y = SplitterPosition.Y / UEngine::GetEngine().GetWindowInfo().Height;;
}

void FSplitterH::OnDrag(FVector2 Position)
{
	int32 WindowHeight = UEngine::GetEngine().GetWindowInfo().Height;
	Position.Y = FMath::Clamp((int32)Position.Y, MinViewportHeight, WindowHeight - MinViewportHeight);

	
	if (TopLeft && BottomLeft && TopRight && BottomRight)
	{
		// 상단 뷰포트의 높이 조절 
		UINT NewHeight = static_cast<UINT>(Position.Y);
		TopLeft->ResizeViewport(TopLeft->GetViewport().Width, NewHeight);
		TopRight->ResizeViewport(TopRight->GetViewport().Width, NewHeight);

		// 하단 뷰포트의 위치와 높이 조절	
		UINT NewHeightBottom = WindowHeight - NewHeight;
		BottomLeft->MoveViewport(BottomLeft->GetViewport().TopLeftX, NewHeight);
		BottomRight->MoveViewport(BottomRight->GetViewport().TopLeftX, NewHeight);
		BottomLeft->ResizeViewport(BottomLeft->GetViewport().Width, NewHeightBottom);
		BottomRight->ResizeViewport(BottomRight->GetViewport().Width, NewHeightBottom);

		SetSplitterPosition(FVector2(SplitterPosition.X, Position.Y));
	}
}

bool FSplitterH::IsMouseOverSplitter(FVector2 MousePosition) const
{
	return (MousePosition.Y >= SplitterPosition.Y - SplitterThickness / 2 &&
		MousePosition.Y <= SplitterPosition.Y + SplitterThickness / 2);
}

void FSplitterH::CalculateInitialSplitterPosition()
{
	// 초기 Splitter 위치를 화면을 4분할하는 위치로 설정
	SplitterPosition = FVector2(TopLeft->GetViewport().TopLeftX, (TopLeft->GetViewport().Height + BottomLeft->GetViewport().TopLeftY) / 2);
}

void FSplitterV::OnDrag(FVector2 Position)
{
	int32 WindowWidth = UEngine::GetEngine().GetWindowInfo().Width;
	Position.X = FMath::Clamp((int32)Position.X, MinViewportWidth, WindowWidth - MinViewportWidth);

	if (TopLeft && BottomLeft && TopRight && BottomRight)
	{
		// 좌측 뷰포트의 너비를 조절
		UINT NewWidth = static_cast<UINT>(Position.X);
		TopLeft->ResizeViewport(NewWidth, TopLeft->GetViewport().Height);
		BottomLeft->ResizeViewport(NewWidth, BottomLeft->GetViewport().Height);

		// 우측 뷰포트의 위치와 너비 조절
		UINT NewWidthRight = WindowWidth - NewWidth;
		TopRight->MoveViewport(NewWidth, TopRight->GetViewport().TopLeftY);
		BottomRight->MoveViewport(NewWidth, BottomRight->GetViewport().TopLeftY);
		TopRight->ResizeViewport(NewWidthRight, TopRight->GetViewport().Height);
		BottomRight->ResizeViewport(NewWidthRight, BottomRight->GetViewport().Height);

		SetSplitterPosition(FVector2(Position.X, SplitterPosition.Y));
	}
}

bool FSplitterV::IsMouseOverSplitter(FVector2 MousePosition) const
{
	return (MousePosition.X >= SplitterPosition.X - SplitterThickness / 2 &&
		MousePosition.X <= SplitterPosition.X + SplitterThickness / 2);
}

void FSplitterV::CalculateInitialSplitterPosition()
{
	// 초기 Splitter 위치를 화면을 4분할하는 위치로 설정
	SplitterPosition = FVector2((TopLeft->GetViewport().Width + TopRight->GetViewport().TopLeftX) / 2, TopLeft->GetViewport().TopLeftY);
}

