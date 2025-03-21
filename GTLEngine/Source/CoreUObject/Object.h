#pragma once

class UClass;

class UObject
{
private:
	UObject(const UObject&) = delete;
	UObject(const UObject&&) = delete;
	UObject& operator=(const UObject&) = delete;
	UObject& operator=(const UObject&&) = delete;

public:
	using Super = UObject;
	using ThisClass = UObject;

	static UClass* StaticClass();

public:
	UObject() = default;
	~UObject() = default;

public:
	virtual void Destroy();

public:
	
	FName GetFName() const { return NamePrivate; }
	FString GetName() const { return NamePrivate.ToString(); }
	inline const uint32 GetUUID() const { return UUID; }
	uint32 GetInternalIndex() const { return InternalIndex; }
	UClass* GetClass() const { return ClassPrivate; }

private:
	friend class FObjectFactory;
	friend class UClass;

	// Name of this object.
	FName NamePrivate;
	uint32 UUID;
	uint32 InternalIndex; // Index of GUObjectArray
	UClass* ClassPrivate;

public:
	/** this가 SomeBase인지, SomeBase의 자식 클래스인지 확인합니다. */
	bool IsA(const UClass* SomeBase) const;

	template<typename T>
	bool IsA() const
	{
		return IsA(T::StaticClass());
	}

};


#include "ObjectMacros.h"
#include "Casts.h"