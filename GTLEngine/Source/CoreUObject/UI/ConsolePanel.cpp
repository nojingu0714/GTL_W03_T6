#include "pch.h"
#include "ConsolePanel.h"

#include "Utils/GTLStringLibrary.h"

UConsolePanel::UConsolePanel(): WindowWidth(360.f), WindowHeight(400.f) {
    strcpy_s(InputBuffer, 256, "");
}

int UConsolePanel::Stricmp(const char* s1, const char* s2) {
    int d;
    while ( (d = toupper(*s2) - toupper(*s1)) == 0 && *s1 ) {
        s1++; s2++;
    }
    return d;
}
int UConsolePanel::Strnicmp(const char* s1, const char* s2, int n) {
    int d = 0;
    while ( n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1 ) {
        s1++; s2++; n--;
    }
    return d;
}

char* UConsolePanel::Strdup(const char* s) {
    IM_ASSERT(s);
    size_t len = strlen(s) + 1;
    void* buf = ImGui::MemAlloc(len);
    IM_ASSERT(buf);
    return (char*)memcpy(buf, (const void*)s, len);
}

void UConsolePanel::Strtrim(char* s) {
    char* str_end = s + strlen(s);
    while ( str_end > s && str_end[-1] == ' ' )
        str_end--;
    *str_end = 0;
}

TArray<std::string> UConsolePanel::StrSplit(std::string s) {
    size_t pos_start = 0, pos_end, delim_len = 1;
    std::string token;
    std::vector<std::string> res;

    while ( (pos_end = s.find(" ", pos_start)) != std::string::npos ) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

FString UConsolePanel::Spawn(TArray<FString> commands) {
    return FString();
}

FString UConsolePanel::Delete(TArray<FString> commands) {
    return FString();
}

void UConsolePanel::Tick(float TickTime) {    
    float scaleX = io.DisplaySize.x / 1600.0f;
    float scaleY = io.DisplaySize.y / 900.0f;
    ImVec2 WinSize(WindowWidth * scaleX, WindowHeight * scaleY);

    ImGui::SetNextWindowPos(ImVec2(5, 450), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(WinSize, ImGuiCond_Appearing);

	ImGui::Begin("Console");

    ClearLogButton();
    LogDisplay();

    ImGui::Separator();

    CommandInput();

	ImGui::End();
}

void UConsolePanel::ClearLogButton()
{
    if (ImGui::Button("Clear"))
        ULogManager::ClearLog();
}

void UConsolePanel::LogDisplay()
{
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar);
    for (const FLogItem& item : ULogManager::GetLogs()) {
		ImGui::Text("%s : %s", UGTLStringLibrary::WStringToString(item.Category->CategoryName).c_str(), item.Message);
    }
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
}

void UConsolePanel::CommandInput()
{
    const ImGuiInputTextFlags INPUT_TEXT_FLAGS = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
    if (ImGui::InputText("Input", InputBuffer, IM_ARRAYSIZE(InputBuffer), INPUT_TEXT_FLAGS, 0, (void*)this)) {
        char* s = InputBuffer;
        Strtrim(s);
        if (s[0])
            ExecCommand(s);
        strcpy_s(s, 256, "");
    }
}

void UConsolePanel::Destroy() {}

void UConsolePanel::ExecCommand(const char* s) {
    //AddLog("# %s\n", s);
    UE_LOG(Log, Display, TEXT("# %s\n"), s);
    StrSplit(s);
}