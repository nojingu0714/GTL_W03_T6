#include "pch.h"
#include "LogManager.h"

DECLARE_LOG_CATEGORY(Log);
DECLARE_LOG_CATEGORY(LogTemp);

void ULogManager::AddLog(FLogCategory* Category, ELogVerbosity Verbosity, const wchar_t* fmt, ...)
{
    wchar_t wbuf[1024];
    va_list args;

    va_start(args, fmt);
    _vsnwprintf_s(wbuf, ARRAYSIZE(wbuf), fmt, args);
    wbuf[ARRAYSIZE(wbuf) - 1] = 0;
    va_end(args);
    char* cbuf = new char[1024];
    WideCharToMultiByte(CP_ACP, 0, wbuf, ARRAYSIZE(wbuf), cbuf, 1024, nullptr, nullptr);
	Get().Items.push_back({ cbuf, Category, Verbosity });
}

void ULogManager::AddLog(FLogCategory* Category, ELogVerbosity Verbosity, FString fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	AddLog(Category, Verbosity, fmt.c_str(), args);
	va_end(args);
}

void ULogManager::ClearLog() {
    TArray<FLogItem> item = Get().Items;
    for (int i = 0; i < item.size(); i++)
    {
        item[i].Message = nullptr;
		item[i].Category = nullptr;
		item[i].Verbosity = ELogVerbosity::NoLogging;
    }
    Get().Items.clear();
}
