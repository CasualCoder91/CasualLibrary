#include <Windows.h>
#include <dbghelp.h>

class Helper {
public:
	static bool is64(HANDLE process);
};