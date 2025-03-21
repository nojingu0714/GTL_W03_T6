#pragma once

#include "MathUtils.h"
#include "Vector.h"
#include "Quat.h"
#include "Matrix.h"

struct FRotator
{
	float Pitch; // Y
	float Yaw; // Z
	float Roll; // X

	static const FRotator ZeroRotator;
	static FRotator MakeFromDirection(const FVector& normal);

	FRotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
	FRotator(float InPitch, float InYaw, float InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}
	FRotator(const FVector& InVector) : Pitch(FMath::RadiansToDegrees(InVector.Y)), Yaw(FMath::RadiansToDegrees(InVector.Z)), Roll(FMath::RadiansToDegrees(InVector.X)) {}
	FRotator(const FRotator& InRotator) : Pitch(InRotator.Pitch), Yaw(InRotator.Yaw), Roll(InRotator.Roll) {}
	//FRotator(const FQuat& InQuat) : FRotator(InQuat.GetEuler()) {}
	FRotator(const FQuat& InQuat); // 언리얼 코드로 수정
	FRotator(const FMatrix InMatrix);

	FRotator operator+(const FRotator& Other) const;
	FRotator& operator+=(const FRotator& Other);

	FRotator operator-(const FRotator& Other) const;
	FRotator& operator-=(const FRotator& Other);

	FRotator operator*(const FRotator& Other) const;
	FRotator operator*(float Scalar) const;
	FRotator& operator*=(float Scalar);

	FRotator operator/(const FRotator& Other) const;
	FRotator operator/(float Scalar) const;
	FRotator& operator/=(float Scalar);

	FRotator operator-() const;

	bool operator==(const FRotator& Other) const;
	bool operator!=(const FRotator& Other) const;

	bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const;
	bool IsZero() const;

	bool Equals(const FRotator& R, float Tolerance = KINDA_SMALL_NUMBER) const;

	FRotator Add(float DeltaPitch, float DeltaYaw, float DeltaRoll);
	FRotator GetInverse() const;

	FQuat Quaternion() const;
	FVector Euler() const;

	FVector TransformRotVecToMatrix(const FVector& V) const;
	FVector UnrotateVector(const FVector& V) const;
	FMatrix ToMatrix() const;

	float Clamp(float Angle) const;
	FRotator GetNormalized() const;
	FRotator GetDenormalized() const;
	void Normalize();

};

inline FRotator FRotator::MakeFromDirection(const FVector& normal) {
	return FRotator();
}


inline FRotator::FRotator(const FQuat& InQuat)
{
	const float SingularityTest = InQuat.Z * InQuat.X - InQuat.W * InQuat.Y;
	const float YawY = 2.f * (InQuat.W * InQuat.Z + InQuat.X * InQuat.Y);
	const float YawX = (1.f - 2.f * (FMath::Square(InQuat.Y) + FMath::Square(InQuat.Z)));

	// reference 
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	// this value was found from experience, the above websites recommend different values
	// but that isn't the case for us, so I went through different testing, and finally found the case 
	// where both of world lives happily. 
	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.f / PI);

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		Pitch = -90.f;
		Yaw = (FMath::Atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = (-Yaw - (2.f * atan2(InQuat.X, InQuat.W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		Pitch = 90.f;
		Yaw = (FMath::Atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = (Yaw - (2.f * FMath::Atan2(InQuat.X, InQuat.W) * RAD_TO_DEG));
	}
	else
	{
		Pitch = (asin(2.f * SingularityTest) * RAD_TO_DEG);
		Yaw = (FMath::Atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = (FMath::Atan2(-2.f * (InQuat.W * InQuat.X + InQuat.Y * InQuat.Z), (1.f - 2.f * (FMath::Square(InQuat.X) + FMath::Square(InQuat.Y)))) * RAD_TO_DEG);
	}
}

inline FRotator FRotator::operator+(const FRotator& Other) const
{
	return { Pitch + Other.Pitch, Yaw + Other.Yaw, Roll + Other.Roll };
}

inline FRotator& FRotator::operator+=(const FRotator& Other)
{
	Pitch += Other.Pitch; Yaw += Other.Yaw; Roll += Other.Roll;
	return *this;
}

inline FRotator FRotator::operator-(const FRotator& Other) const
{
	return { Pitch - Other.Pitch, Yaw - Other.Yaw, Roll - Other.Roll };
}

inline FRotator& FRotator::operator-=(const FRotator& Other)
{
	Pitch -= Other.Pitch; Yaw -= Other.Yaw; Roll -= Other.Roll;
	return *this;
}

inline FRotator FRotator::operator*(const FRotator& Other) const
{
	return { Pitch * Other.Pitch, Yaw * Other.Yaw, Roll * Other.Roll };
}

inline FRotator FRotator::operator*(float Scalar) const
{
	return { Pitch * Scalar, Yaw * Scalar, Roll * Scalar };
}

inline FRotator& FRotator::operator*=(float Scalar)
{
	Pitch *= Scalar; Yaw *= Scalar; Roll *= Scalar;
	return *this;
}

inline FRotator FRotator::operator/(const FRotator& Other) const
{
	return { Pitch / Other.Pitch, Yaw / Other.Yaw, Roll / Other.Roll };
}

inline FRotator FRotator::operator/(float Scalar) const
{
	return { Pitch / Scalar, Yaw / Scalar, Roll / Scalar };
}

inline FRotator& FRotator::operator/=(float Scalar)
{
	Pitch /= Scalar; Yaw /= Scalar; Roll /= Scalar;
	return *this;
}

inline FRotator FRotator::operator-() const
{
	return { -Pitch, -Yaw, -Roll };
}

inline bool FRotator::operator==(const FRotator& Other) const
{
	return Pitch == Other.Pitch && Yaw == Other.Yaw && Roll == Other.Roll;
}

inline bool FRotator::operator!=(const FRotator& Other) const
{
	return !(*this == Other);
}

inline FRotator operator*(float Scalar, const FRotator& Rotator)
{
	return Rotator * Scalar;
}

inline bool FRotator::IsNearlyZero(float Tolerance) const
{
	return FMath::Abs(Pitch) <= Tolerance && FMath::Abs(Yaw) <= Tolerance && FMath::Abs(Roll) <= Tolerance;
}

inline bool FRotator::IsZero() const
{
	return Pitch == 0.0f && Yaw == 0.0f && Roll == 0.0f;
}

inline bool FRotator::Equals(const FRotator& R, float Tolerance) const
{
	return FMath::Abs(Pitch - R.Pitch) <= Tolerance && FMath::Abs(Yaw - R.Yaw) <= Tolerance && FMath::Abs(Roll - R.Roll) <= Tolerance;
}

inline FRotator FRotator::Add(float DeltaPitch, float DeltaYaw, float DeltaRoll)
{
	return { Pitch + DeltaPitch, Yaw + DeltaYaw, Roll + DeltaRoll };
}

inline FRotator FRotator::GetInverse() const
{
	return { -Pitch, -Yaw, -Roll };
}

inline FQuat FRotator::Quaternion() const
{
	float DegToRad = PI / 180.0f;
	float Div =  DegToRad / 2.0f;
	float SP, SY, SR;
	float CP, CY, CR;

	FMath::SinCos(&SP, &CP, Pitch * Div);
	FMath::SinCos(&SY, &CY, Yaw * Div);
	FMath::SinCos(&SR, &CR, Roll * Div);
	
	FQuat RotationQuat;
	RotationQuat.X = CR * SP * SY - SR * CP * CY;
	RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
	RotationQuat.Z = CR * CP * SY - SR * SP * CY;
	RotationQuat.W = CR * CP * CY + SR * SP * SY;

	return RotationQuat;
}

inline FVector FRotator::Euler() const
{
	return FVector(Roll, Pitch, Yaw);
}

// Rotate a vector rotated by this rotator
inline FVector FRotator::TransformRotVecToMatrix(const FVector& V) const
{
	return FMatrix(*this).TransformPositionVector(V);
}

inline FVector FRotator::UnrotateVector(const FVector& V) const
{
	return (FMatrix(*this).Inverse().TransformPositionVector(V));
}

inline FMatrix FRotator::ToMatrix() const
{
	return (FMatrix(*this));
}

inline float FRotator::Clamp(float Angle) const
{
	Angle = std::fmod(Angle, 360.0f);

	if (Angle < 0.0f)
	{
		Angle += 360.0f;
	}
	return Angle;
}

inline FRotator FRotator::GetNormalized() const
{
	return { FMath::UnwindDegrees(Pitch), FMath::UnwindDegrees(Yaw), FMath::UnwindDegrees(Roll) };
}

inline FRotator FRotator::GetDenormalized() const
{
	return { FMath::UnwindDegrees(Pitch), FMath::UnwindDegrees(Yaw), FMath::UnwindDegrees(Roll) };
}

inline void FRotator::Normalize()
{
	Pitch = FMath::UnwindDegrees(Pitch);
	Yaw = FMath::UnwindDegrees(Yaw);
	Roll = FMath::UnwindDegrees(Roll);
}

