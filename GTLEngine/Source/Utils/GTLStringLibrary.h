#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <string>
#include <codecvt>
#include <locale>

class UGTLStringLibrary
{
public:
	static FString StringToWString(const std::string& str) {
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0);
		FString wstr(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], size_needed);
		return wstr;
	}

	static std::string WStringToString(const FString& wstr) {
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
		std::string str(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &str[0], size_needed, nullptr, nullptr);
		return str;
	}

	static std::string StringTrimFrontBack(const std::string& InString)
	{
		// 앞뒤 공백 제거.
		std::string TrimmedString = InString;
		size_t StartPos = TrimmedString.find_first_not_of(" \t");
		size_t EndPos = TrimmedString.find_last_not_of(" \t");
		TrimmedString = TrimmedString.substr(StartPos, EndPos - StartPos + 1);
		return TrimmedString;
	}

	static std::string StringRemoveComment(const std::string& InString)
	{
		// 주석 제거.
		size_t CommentPos = InString.find_first_of('#');
		if (CommentPos != std::string::npos)
		{
			return InString.substr(0, CommentPos);
		}
		return InString;
	}

	static std::string StringRemoveNoise(const std::string& InString)
	{
		// 노이즈 제거.
		std::string RetString = StringRemoveComment(InString);
		return StringTrimFrontBack(RetString);
	}

};