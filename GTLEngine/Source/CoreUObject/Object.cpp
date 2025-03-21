#include "pch.h"
#include "Object.h"

#include "Engine/Engine.h"
#include "Class.h"

UClass* UObject::StaticClass()
{
    static std::unique_ptr<UClass, UClassDeleter> StaticClassInfo = nullptr;
    if (!StaticClassInfo)
    {
        constexpr size_t ClassSize = sizeof(UClass);
        void* RawMemory = FPlatformMemory::Malloc(ClassSize);
        UClass* ClassPtr = new(RawMemory) UClass{ L"UObject", sizeof(UObject), alignof(UObject), nullptr };
        StaticClassInfo = std::unique_ptr<UClass, UClassDeleter>(ClassPtr, UClassDeleter{});
    }
    return StaticClassInfo.get();
}

void UObject::Destroy()
{
}

bool UObject::IsA(const UClass* SomeBase) const
{
    const UClass* ThisClass = GetClass();
    return ThisClass->IsChildOf(SomeBase);
}