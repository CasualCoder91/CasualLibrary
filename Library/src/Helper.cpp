#include "Helper.h"

BOOL Helper::isWow64(const HANDLE process){
    BOOL bIsWow64 = FALSE;

    typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process"));

    if (NULL != fnIsWow64Process) {
        if (!fnIsWow64Process(process, &bIsWow64)) {
            //handle error
        };
    };
    return bIsWow64;
}

bool Helper::is86(const HANDLE process){
    SYSTEM_INFO systemInfo = { 0 };
    GetNativeSystemInfo(&systemInfo);

    // x86 environment
    if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
        return true;

    // Check if the process is an x86 process that is running on x64 environment.
    // IsWow64 returns true if the process is an x86 process
    return isWow64(process);
}

bool Helper::is64(const HANDLE pHandle){
    return !is86(pHandle);
}

bool Helper::matchingBuilt(const HANDLE process){
    #if defined(_WIN64)
        bool app64 = true;
    #else
        bool app64 = false;
    #endif
    return is64(process) == app64;
}
