#pragma once

#include <string>
#include <codecvt>
#include <locale>

class UGTLStringLibrary
{
public:
	static std::wstring StringToWString(const std::string& str)
	{
		std::string ret = str;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(ret);
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