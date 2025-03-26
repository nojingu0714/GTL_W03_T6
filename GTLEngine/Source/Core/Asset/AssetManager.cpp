#include "pch.h"
#include "AssetManager.h"

#include <filesystem>
#include "Utils/GTLStringLibrary.h"

TArray<FString> FAssetManager::ObjFileNames;

void FAssetManager::InitAssetManager()
{
	PreLoadObjFiles();
}

void FAssetManager::PreLoadObjFiles()
{
	
	std::string BasePathName = "Contents/";

	// Obj 파일 로드
	
	for (const auto& Entry : std::filesystem::recursive_directory_iterator(BasePathName))
	{
		if (Entry.is_regular_file() && Entry.path().extension() == ".obj")
		{
			ObjFileNames.push_back(UGTLStringLibrary::StringToWString(Entry.path().string()));
		}
	}
}
