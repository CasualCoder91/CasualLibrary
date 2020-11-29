#include "Internal.hpp"

[[nodiscard]] Address Memory::Internal::getModule(const char* mod) noexcept {
    MODULEENTRY32 modEntry{ sizeof(MODULEENTRY32) };
    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, 0);

    if (Module32First(snapshot, &modEntry)) {
        do {
            if (!strcmp(mod, static_cast<char*>(modEntry.szModule))) {
                CloseHandle(snapshot);
                return reinterpret_cast<uintptr_t>(modEntry.hModule);
            }
        } while (Module32Next(snapshot, &modEntry));
    }

    return nullptr;
}

[[nodiscard]] Address Memory::Internal::getAddress(uintptr_t addr, const std::vector<uintptr_t>& vect) noexcept {
    for (size_t i = 0; i < vect.size(); i++) {
        addr = read<uintptr_t>(addr);
        addr += vect[i];
    }
    return addr;
}

[[nodiscard]] Address Memory::Internal::getAddress(const Address& basePrt, const std::vector<uintptr_t>& offsets) noexcept {
    return getAddress(basePrt.get(), offsets);
}

[[nodiscard]] Address Memory::Internal::findSignature(const uintptr_t modAddr, const char* sig, const size_t size) noexcept {
    uint8_t* const scanBytes = reinterpret_cast<uint8_t*>(modAddr);
    std::vector<int> patternBytes = patternToBytes(sig);
    const size_t s = patternBytes.size();
    int* const data = patternBytes.data();

    for (size_t i = 0u; i < size - s; ++i) {
        bool found = true;

        for (size_t j = 0u; j < s; ++j) {
            if (scanBytes[i + j] != data[j] && data[j] != -1)
            {
                found = false;
                break;
            }
        }
        if (found)
            return &scanBytes[i];
    }
    return nullptr;
}

[[nodiscard]] Address Memory::Internal::findSignature(const Address& modAddr, const char* sig, const size_t size) noexcept {
    return findSignature(modAddr.get(), sig, size);
}

[[nodiscard]] Address Memory::Internal::findModuleSignature(const char* mod, const char* sig) noexcept {
    const uintptr_t moduleHandle = getModule(mod).get();

    if (mod) {
        const PIMAGE_DOS_HEADER idh = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleHandle);
        const PIMAGE_NT_HEADERS inh = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(moduleHandle) + idh->e_lfanew);
        return findSignature(moduleHandle, sig, inh->OptionalHeader.SizeOfImage);
    }

    return nullptr;
}