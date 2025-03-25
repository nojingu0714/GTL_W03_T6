#pragma once

#define NOMINMAX

// STL include
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <cmath>


// D3D include
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK/Libs/Debug/DirectXTK.lib")
#pragma comment(lib, "DirectXTex/Libs/Debug/DirectXTex.lib")

// 디버깅 시 콘솔 찍기.
#pragma comment( linker, "/entry:WinMainCRTStartup /subsystem:console" )
#include <iostream>

#else
#pragma comment(lib, "DirectXTK/Libs/Release/DirectXTK.lib")
#pragma comment(lib, "DirectXTex/Libs/Release/DirectXTex.lib")
#endif

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

using namespace Microsoft::WRL;

// typedef default int type. 
// TODO: std로 바꿔주기.
typedef UINT		uint;
typedef	UINT8		uint8;
typedef	UINT16		uint16;
typedef	UINT32		uint32;
typedef	UINT64		uint64;
typedef	INT8		int8;
typedef	INT16		int16;
typedef	INT32		int32;
typedef	INT64		int64;

typedef char ANSICHAR;
typedef wchar_t WIDECHAR;

template <typename T>
using TArray = std::vector<T>;

template <typename T>
using TList = std::list<T>;

template <typename T1, typename T2>
using TPair = std::pair<T1, T2>;

template <typename T1, typename T2>
using TMap = std::unordered_map<T1, T2>;

template <typename T>
using TQueue = std::queue<T>;

template <typename T>
using TSet = std::set<T>;

using FString = std::wstring;

// inline을 강제하는 매크로
#define FORCEINLINE __forceinline

// inline을 하지않는 매크로
#define FORCENOINLINE __declspec(noinline)

struct FWindowInfo
{
    uint Id;
    HWND WindowHandle;
    uint Width;
    uint Height;
    uint Top;
    uint Bottom;
    uint Left;
    uint Right;
    RECT screenRect;
};

template <typename T>
void SetFlagOff(T& target, const T flag) {
	target = static_cast<T>(static_cast<uint32>(target) & ~static_cast<uint32>(flag));
}

template <typename T>
void SetFlagOn(T& target, const T flag) {
	target = static_cast<T>(static_cast<uint32>(target) | static_cast<uint32>(flag));
}

template <typename T>
bool GetFlag(T& target, const T flag) {
	return ((static_cast<uint32>(target) & static_cast<uint32>(flag)) != 0);
}


#include "NameTypes.h"
#include "LogManager.h"

#include "UObject/UObjectIterator.h"
