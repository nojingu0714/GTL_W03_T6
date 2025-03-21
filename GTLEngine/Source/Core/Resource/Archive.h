#pragma once

#include <fstream>
#include "SimpleJSON/json.hpp"

using FString = std::wstring;

class FJsonArchive
{
public:
	enum class EMode
	{
		Saving,
		Loading
	};

private:
	json::JSON JsonData;
	EMode Mode;
	FString FilePath;

public:
	FJsonArchive(const FString& FileName, EMode InMode)
		: Mode(InMode), FilePath(FileName)
	{
		if (Mode == EMode::Loading)
		{
			std::ifstream File(FilePath);
			if (File.is_open())
			{
				std::string JsonString((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
				JsonData = json::JSON::Load(JsonString);
				File.close();
			}
		}
	}

	void SaveToFile()
	{
		if (Mode == EMode::Saving)
		{
			std::ofstream OutFile(FilePath);
			if (OutFile.is_open())
			{
				OutFile << JsonData.dump(4);
				OutFile.close();
			}
		}
	}

	template<typename T>
	FJsonArchive& operator<<(const T& Value)
	{
		if (Mode == EMode::Saving)
		{
			JsonData[Value.first] = Value.second;
		}
		return *this;
	}



};