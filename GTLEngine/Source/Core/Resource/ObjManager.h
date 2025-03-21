#pragma once

#include "Mesh/StaticMeshAsset.h"
#include "Mesh/UStaticMesh.h"
#include "ObjectFactory.h"
#include "FileLoader.h"

template<typename TObject>
class FObjectIterator
{
private:

public:
    //  생성자: UObject 배열 크기 가져오기
    FObjectIterator()
        : CurrentIndex(0), ObjectArray(GUObjectArray)
    {
        NumObjects = ObjectArray.GetObjectArrayNum();
        AdvanceToNextValidObject();
    }

    //  다음 유효한 객체로 이동
    void AdvanceToNextValidObject()
    {
        while (CurrentIndex < NumObjects)
        {
            FUObjectItem* ObjectItem = ObjectArray.IndexToObject(CurrentIndex);
            if (ObjectItem && ObjectItem->Object && ObjectItem->Object->IsA<TObject>() && !ObjectItem->Object->IsPendingKill())
            {
                return;
            }
            ++CurrentIndex;
        }
    }

    //  반복자 증가 연산자 (++It)
    FObjectIterator& operator++()
    {
        ++CurrentIndex;
        AdvanceToNextValidObject();
        return *this;
    }

    //  현재 객체 반환 (*It)
    TObject* operator*() const
    {
        FUObjectItem* ObjectItem = ObjectArray.IndexToObject(CurrentIndex);
        return ObjectItem ? static_cast<TObject*>(ObjectItem->Object) : nullptr;
    }

    //  반복 종료 여부 확인 (while(It)에서 사용)
    explicit operator bool() const
    {
        return CurrentIndex < NumObjects;
    }
};

class FObjManager
{
private:
    static TMap<FString, FStaticMesh*> ObjStaticMeshMap; // TMap 기반의 메시 데이터

public:
    // FStaticMesh를 로드하는 함수 (Cooked Data)
    static FStaticMesh* LoadObjStaticMeshAsset(const FString& PathFileName)
    {
        auto It = ObjStaticMeshMap.find(PathFileName);
        if (It != ObjStaticMeshMap.end())
        {
            return It->second; // 기존에 로드된 메시 반환
        }

        // OBJ 파싱 후 새로운 FStaticMesh 생성
        FStaticMesh* NewFStaticMesh = new FStaticMesh();
        NewFStaticMesh->PathFileName = PathFileName;

        ObjStaticMeshMap[PathFileName] = NewFStaticMesh;
        return NewFStaticMesh;
    }

    // UStaticMesh를 로드하는 함수 (언리얼 에디터에서 활용)
    static UStaticMesh* LoadObjStaticMesh(const FString& PathFileName)
    {
        // `FObjectIterator<UStaticMesh>`를 사용하여 기존 StaticMesh 찾기
        for (FObjectIterator<UStaticMesh> It; It; ++It)
        {
            UStaticMesh* StaticMesh = *It;
            if (StaticMesh && StaticMesh->GetAssetPathName() == PathFileName)
            {
                return StaticMesh; // 기존 UStaticMesh 반환
            }
        }

        // 기존 StaticMesh가 없으면 새로 생성
        FStaticMesh* Asset = FObjManager::LoadObjStaticMeshAsset(PathFileName);
        if (!Asset)
        {
            return nullptr; // OBJ 로드 실패 시 nullptr 반환
        }

        // ConstructObject<UStaticMesh>()를 사용하여 새 StaticMesh 생성
        UStaticMesh* NewStaticMesh = FObjectFactory::ConstructObject<UStaticMesh>();
        if (NewStaticMesh)
        {
            NewStaticMesh->SetStaticMeshAsset(Asset);
        }

        return NewStaticMesh;
    }
};