#pragma once

#include "EngineStatics.h"
#include <string>
#include "HAL/PlatformMemory.h"
#include "UObject/UObjectArray.h"

class UObject;

class FObjectFactory
{
public:
    /**
     * UObject를 생성합니다.
     * @tparam T UObject를 상속받은 클래스
     * @return 캐스팅된 UObject*
     */
    template<typename T>
        requires std::derived_from<T, UObject>
    static T* ConstructObject()
    {
        constexpr size_t ObjectSize = sizeof(T);
        void* RawMemory = FPlatformMemory::Malloc(ObjectSize);

        T* ObjectPtr = new (RawMemory) T();
        ObjectPtr->UUID = UEngineStatics::GenUUID();

        ObjectPtr->NamePrivate = T::StaticClass()->GetName() + L"_" + std::to_wstring(ObjectPtr->UUID);
        ObjectPtr->ClassPrivate = T::StaticClass();
        GUObjectArray.push_back(ObjectPtr);
		ObjectPtr->InternalIndex = static_cast<int>(GUObjectArray.size() - 1);

        return ObjectPtr;
    }

    template<typename T>
		requires std::derived_from<T, UObject>
	static void DestroyObject(T* Object)
	{
		// GUObjectArray에서 제거.
		GUObjectArray.erase(std::remove(GUObjectArray.begin(), GUObjectArray.end(), Object), GUObjectArray.end());

		Object->~T();
		FPlatformMemory::Free(Object, sizeof(T));
		Object = nullptr;
	}
};
