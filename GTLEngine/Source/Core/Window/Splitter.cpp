#include "pch.h"
#include "Splitter.h"
#include "Viewport.h"

void FSplitterH::OnDrag(FVector2 Position)
{
	if (TopLeft && BottomLeft && TopRight && BottomRight)
	{
		// 상단 뷰포트의 높이 조절 
		UINT NewHeight = static_cast<UINT>(Position.Y);
		TopLeft->ResizeViewport(TopLeft->GetViewport().Width, NewHeight);
		TopRight->ResizeViewport(TopRight->GetViewport().Width, NewHeight);

		// 하단 뷰포트의 위치와 높이 조절	
		UINT NewHeightBottom = BottomLeft->GetViewport().Height + BottomLeft->GetViewport().TopLeftY - NewHeight;
		BottomLeft->MoveViewport(BottomLeft->GetViewport().TopLeftX, NewHeight);
		BottomRight->MoveViewport(BottomRight->GetViewport().TopLeftX, NewHeight);
		BottomLeft->ResizeViewport(BottomLeft->GetViewport().Width, NewHeightBottom);
		BottomRight->ResizeViewport(BottomRight->GetViewport().Width, NewHeightBottom);

		SetSplitterPosition(Position);
	}
	
}

void FSplitterH::CalculateInitialSplitterPosition()
{
	// 초기 Splitter 위치를 화면을 4분할하는 위치로 설정
	SplitterPosition = FVector2(TopLeft->GetViewport().TopLeftX, (TopLeft->GetViewport().Height + BottomLeft->GetViewport().TopLeftY) / 2);
}

void FSplitterV::OnDrag(FVector2 Position)
{
	if (TopLeft && BottomLeft && TopRight && BottomRight)
	{
		// 좌측 뷰포트의 너비를 조절
		UINT NewWidth = static_cast<UINT>(Position.X);
		TopLeft->ResizeViewport(NewWidth, TopLeft->GetViewport().Height);
		BottomLeft->ResizeViewport(NewWidth, BottomLeft->GetViewport().Height);

		// 우측 뷰포트의 위치와 너비 조절
		UINT NewWidthRight = TopRight->GetViewport().Width + TopRight->GetViewport().TopLeftX - NewWidth;
		TopRight->MoveViewport(NewWidth, TopRight->GetViewport().TopLeftY);
		BottomRight->MoveViewport(NewWidth, BottomRight->GetViewport().TopLeftY);
		TopRight->ResizeViewport(NewWidthRight, TopRight->GetViewport().Height);
		BottomRight->ResizeViewport(NewWidthRight, BottomRight->GetViewport().Height);

		SetSplitterPosition(Position);
	}
}

void FSplitterV::CalculateInitialSplitterPosition()
{
	// 초기 Splitter 위치를 화면을 4분할하는 위치로 설정
	SplitterPosition = FVector2((TopLeft->GetViewport().Width + TopRight->GetViewport().TopLeftX) / 2, TopLeft->GetViewport().TopLeftY);
}
