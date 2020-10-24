#include <Windows.h>
#include <dbghelp.h> //needed for ImageNtHeader

#pragma comment(lib, "dbghelp.lib")

class Helper {
public:
	static bool is64(HANDLE process);
};