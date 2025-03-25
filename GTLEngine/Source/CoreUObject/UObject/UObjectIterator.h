#pragma once

#include "CoreUObject/Class.h"
#include "UObjectArray.h"

class UObject;

void GetObjectOfClass(const UClass* Class, TArray<UObject*>& OutObjects);

template<typename T>
class TObjectIterator
{

public:
	TObjectIterator() : Index(0)
	{
		GetObjectOfClass(T::StaticClass(), ObjectArray);
	}
	TObjectIterator(const TObjectIterator& Other) : Index(Other.Index) {}

	void AdvanceToNextValidObject()
	{
		while (Index < ObjectArray.size() && !ObjectArray[Index]->IsA(T::StaticClass()))
		{
			Index++;
		}
	}


	TObjectIterator& operator=(const TObjectIterator& Other)
	{
		Index = Other.Index;
		return *this;
	}
	TObjectIterator& operator++()
	{
		Index++;
		return *this;
	}
	TObjectIterator operator++(int)
	{
		TObjectIterator Temp = *this;
		Index++;
		return Temp;
	}

	operator bool() const
	{
		return Index >= 0 && Index < ObjectArray.size();
	}
	bool operator==(const TObjectIterator& Other) const
	{
		return Index == Other.Index;
	}
	bool operator!=(const TObjectIterator& Other) const
	{
		return Index != Other.Index;
	}
	T* operator*() const
	{
		return static_cast<T*>(ObjectArray[Index]);
	}
	T* operator->() const
	{
		return static_cast<T*>(ObjectArray[Index]);
	}
	bool operator !() const
	{
		return !(bool)*this;
	}

protected:
	TArray<UObject*> ObjectArray;
	int32 Index;

};