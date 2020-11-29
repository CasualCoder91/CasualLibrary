#include "Helper.hpp"

namespace {
    using LPFN_ISWOW64PROCESS = BOOL(WINAPI*)(HANDLE, PBOOL);

    bool isWow64(const HANDLE process) noexcept {
        BOOL bIsWow64 = FALSE;

        //typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
        LPFN_ISWOW64PROCESS fnIsWow64Process;
        fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process"));

        if (NULL != fnIsWow64Process) {
            if (!fnIsWow64Process(process, &bIsWow64)) {
                // TODO: handle error
            }
        }

        return static_cast<bool>(bIsWow64);
    }

    bool is86(const HANDLE process) noexcept {
        SYSTEM_INFO systemInfo = { 0 };
        GetNativeSystemInfo(&systemInfo);

        // x86 environment
        if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
            return true;

        // Check if the process is an x86 process that is running on x64 environment.
        // IsWow64 returns true if the process is an x86 process
        return isWow64(process);
    }

    bool is64(const HANDLE pHandle) noexcept {
        return !is86(pHandle);
    }
}

[[nodiscard]] bool Helper::matchingBuilt(const HANDLE process) noexcept {
    bool app64 = false;

#ifdef _WIN64
    app64 = true;
#endif

    return is64(process) == app64;
}

#pragma warning(pop)