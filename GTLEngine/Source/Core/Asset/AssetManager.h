#pragma once

#include "Object.h"

class FSceneManager;
class FObjManager;

/*
	프로그램에서 사용할 모든 에셋을 관리할 매니저.


*/
class FAssetManager
{

	static TArray<FString> ObjFileNames;

public:
	void InitAssetManager();

	TArray<FString> GetObjFileNames() const { return ObjFileNames; }

private:
	void PreLoadObjFiles();

public:
	static FAssetManager& Get()
	{
		static FAssetManager Instance;
		return Instance;
	}

	FSceneManager* GetSceneManager() const { return SceneManager; }

private:
	FSceneManager* SceneManager;
	FObjManager* ObjManager;
};