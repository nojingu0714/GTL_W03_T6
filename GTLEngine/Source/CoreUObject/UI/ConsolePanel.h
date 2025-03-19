#pragma once
#include "CoreUObject/UI/UIBase.h"

class UConsolePanel: public UUIBase 
{
	DECLARE_CLASS(UConsolePanel, UUIBase)

public:
	UConsolePanel();
	// UUIBase을(를) 통해 상속됨
	void Tick(float TickTime) override;
	void ClearLogButton();
	void LogDisplay();
	void CommandInput();
	void Destroy() override;

	void ExecCommand(const char* s);
private:
	float WindowWidth;
	float WindowHeight;
	char InputBuffer[256];

	int Stricmp(const char* s1, const char* s2);
	int Strnicmp(const char* s1, const char* s2, int n);
	char* Strdup(const char* s);
	void Strtrim(char* s);
	TArray<std::string> StrSplit(std::string s);

	// command functions
	FString Spawn(TArray<FString> commands);
	FString Delete(TArray<FString> commands);
	ImGuiIO& io = ImGui::GetIO();
};