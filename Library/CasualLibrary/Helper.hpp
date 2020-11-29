#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <dbghelp.h> // ImageNtHeader

#pragma comment(lib, "dbghelp.lib")

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 6387)
#endif

namespace Helper {
	[[nodiscard]] bool matchingBuilt(const HANDLE process) noexcept;
}