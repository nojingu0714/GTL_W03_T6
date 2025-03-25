#pragma once
#include "Math/Vector.h"

class FViewport;

class FSplitter
{
public:
	void Init(FViewport* InTopLeft, FViewport* InTopRight, FViewport* InBottomLeft, FViewport* InBottomRight)
	{
		TopLeft = InTopLeft;
		TopRight = InTopRight;
		BottomLeft = InBottomLeft;
		BottomRight = InBottomRight;

		CalculateInitialSplitterPosition();
		SplitterThickness = 10.f;
	}
	void SetSplitterPosition(FVector2 Position) { SplitterPosition = Position; }
	void SetSplitterThickness(float Thickness) { SplitterThickness = Thickness; }
	virtual void OnDrag(FVector2 Position) = 0;
	virtual bool IsMouseOverSplitter(FVector2 MousePOsition) const = 0;
protected:
	FViewport* TopLeft;
	FViewport* TopRight;
	FViewport* BottomLeft;
	FViewport* BottomRight;

	FVector2 SplitterPosition;
	float SplitterThickness; 

	virtual void CalculateInitialSplitterPosition() = 0;
	
};


class FSplitterH : public FSplitter
{
public:
	void Init(FViewport* InTopLeft, FViewport* InTopRight, FViewport* InBottomLeft, FViewport* InBottomRight)
	{
		FSplitter::Init(InTopLeft, InTopRight, InBottomLeft, InBottomRight);
	}
	void OnDrag(FVector2 Position) override;
	bool IsMouseOverSplitter(FVector2 MousePosition) const override
	{
		return (MousePosition.Y >= SplitterPosition.Y - SplitterThickness / 2 &&
			MousePosition.Y <= SplitterPosition.Y + SplitterThickness / 2);
	}

protected:
	void CalculateInitialSplitterPosition() override;
};

class FSplitterV : public FSplitter
{
public:
	void Init(FViewport* InTopLeft, FViewport* InTopRight, FViewport* InBottomLeft, FViewport* InBottomRight)
	{
		FSplitter::Init(InTopLeft, InTopRight, InBottomLeft, InBottomRight);
	}
	void OnDrag(FVector2 Position) override;
	bool IsMouseOverSplitter(FVector2 MousePosition) const override
	{
		return (MousePosition.X >= SplitterPosition.X - SplitterThickness / 2 &&
			MousePosition.X <= SplitterPosition.X + SplitterThickness / 2);
	}

protected:
	void CalculateInitialSplitterPosition() override;
};
