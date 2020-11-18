#pragma once
#include <Windows.h>
#include <dbghelp.h> //needed for ImageNtHeader

#pragma comment(lib, "dbghelp.lib")


class Helper {
public:
	static bool matchingBuilt(const HANDLE process);
private:
	static bool is64(const HANDLE process);
	static bool is86(const HANDLE process);
	static BOOL isWow64(const HANDLE process);
};