#pragma once
#include "EnginePch.h"

enum class ELogVerbosity {
	NoLogging,
	Fatal,
	Error,
	Warning,
	Display,
	Log,
	LogTemp,
	Verbose,
	VeryVerbose,
	All
};

struct FLogCategory {
	FString CategoryName;
	ELogVerbosity Verbosity = ELogVerbosity::Log;
};

#define DECLARE_LOG_CATEGORY(Name) FLogCategory Name = { TEXT(#Name), ELogVerbosity::Log}
#define DECLARE_LOG_CATEGORY_EXTERN(Name) extern FLogCategory Name

struct FLogItem
{
	char* Message;
	FLogCategory* Category;
	ELogVerbosity Verbosity;
};

class ULogManager {
private:
	ULogManager() = default;
	~ULogManager() = default;
	ULogManager(ULogManager&) = delete;
	ULogManager& operator=(ULogManager&) = delete;
	ULogManager(ULogManager&&) = delete;
	ULogManager& operator=(ULogManager&&) = delete;
public:
	static ULogManager& Get() {
		static ULogManager Instance;
		return Instance;
	}

private:
	TArray<FLogItem> Items;

public:
	static void AddLog(FLogCategory* Category, ELogVerbosity Verbosity, const wchar_t* fmt, ...);
	static void AddLog(FLogCategory* Category, ELogVerbosity Verbosity, FString fmt, ...);
	static void ClearLog();
	inline static TArray<FLogItem> GetLogs() { return Get().Items; }
};

DECLARE_LOG_CATEGORY_EXTERN(Log);
DECLARE_LOG_CATEGORY_EXTERN(LogTemp);


#define UE_LOG(Category, Verbosity, fmt, ...) \
    ULogManager::AddLog(&Category, ELogVerbosity::Verbosity, fmt, ##__VA_ARGS__)
