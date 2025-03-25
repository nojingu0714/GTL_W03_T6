#include "pch.h"
#include "UObjectIterator.h"

void GetObjectOfClass(const UClass* Class, TArray<UObject*>& OutObjects)
{
	for (UObject* Object : GUObjectArray)
	{
		if (Object->IsA(Class))
		{
			OutObjects.push_back(Object);
		}
	}
}
