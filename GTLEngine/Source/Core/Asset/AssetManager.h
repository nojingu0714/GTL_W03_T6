#pragma once

#include "Object.h"

class USceneManager;
class UObjManager;


/*
	프로그램에서 사용할 모든 에셋을 관리할 매니저.


*/
class UAssetManager : public UObject
{
	DECLARE_CLASS(UAssetManager, UObject)



public:

	UAssetManager();
	virtual void Destroy() override;

public:
	UAssetManager& GetInstance()
	{
		static UAssetManager Instance;
		return Instance;
	}


private:
	USceneManager* SceneManager;
	UObjManager* ObjManager;
};