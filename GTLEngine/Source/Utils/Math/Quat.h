#pragma once
#include "Vector.h"

#define PIDIV2 3.141592654/2

struct alignas(16) FQuat : public FVector4 {
    using FVector::X;
    using FVector::Y;
    using FVector::Z;
    using FVector4::W;

    FQuat() : FVector4(0.0f, 0.0f, 0.0f, 1.0f) {}
    explicit FQuat(float InX, float InY, float InZ, float InW) : FVector4(InX, InY, InZ, InW) {}
    explicit FQuat(FVector Rotation) : FVector4(EulerToQuaternion(Rotation)) {}
    FQuat(const FVector& Axis, float AngleInDegrees) : FVector4(AxisAngleToQuaternion(Axis, AngleInDegrees)) {}

	static FQuat EulerToQuaternion(FVector Euler); // 오일러 각을 쿼터니언으로 변환합니다.
	static FVector QuaternionToEuler(const FQuat& quaternion); // 쿼터니언을 오일러 각으로 변환합니다.
	static FQuat AxisAngleToQuaternion(const FVector& Axis, float AngleInDegrees); // 축과 각도로 쿼터니언을 생성합니다.

	static FQuat AddQuaternions(const FQuat& q1, const FQuat& q2); // 두 쿼터니언을 더합니다.
	static FQuat MultiplyQuaternions(const FQuat& q1, const FQuat& q2); // 두 쿼터니언을 곱합니다.
	static FQuat SubtractQuaternions(const FQuat& q1, const FQuat& q2); // 두 쿼터니언을 뺍니다.

	static FQuat MakeFromRotationMatrix(const struct FMatrix& M); // 회전 행렬로부터 쿼터니언을 생성합니다.
	FVector GetEuler() const { return QuaternionToEuler(*this); } // 쿼터니언을 오일러 각으로 변환합니다.

	void Normalize();
};