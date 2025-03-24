#include "pch.h"
#include "Vector.h"

const FVector FVector::ZeroVector = FVector(0, 0, 0);
const FVector FVector::OneVector = FVector(1, 1, 1);

const FVector FVector::UpVector = FVector(0, 0, 1);
const FVector FVector::DownVector = FVector(0, 0, -1);
const FVector FVector::ForwardVector = FVector(1, 0, 0);
const FVector FVector::BackwardVector = FVector(-1, 0, 0);
const FVector FVector::RightVector = FVector(0, 1, 0);
const FVector FVector::LeftVector = FVector(0, -1, 0);

//const FVector FVector::UpVector = FVector(0, 1, 0);
//const FVector FVector::DownVector = FVector(0, -1, 0);
//const FVector FVector::ForwardVector = FVector(0, 0, 1);
//const FVector FVector::BackwardVector = FVector(0, 0, -1);
//const FVector FVector::RightVector = FVector(1, 0, 0);
//const FVector FVector::LeftVector = FVector(-1, 0, 0);

const FVector FVector::XAxisVector = FVector(1, 0, 0);
const FVector FVector::YAxisVector = FVector(0, 1, 0);
const FVector FVector::ZAxisVector = FVector(0, 0, 1);

const FVector2 FVector2::ZeroVector = FVector2(0, 0);
const FVector2 FVector2::UnitVector = FVector2(1, 1);
const FVector2 FVector2::OneVector = FVector2(1, 1);
