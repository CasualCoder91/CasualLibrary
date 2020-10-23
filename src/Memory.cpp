#include "Memory.h"
#include<iostream>
#include<TlHelp32.h>

namespace External {

    Memory::Memory(){
        handle = NULL;
    }

    Memory::Memory(std::string proc) {
        getProcess(proc);
    }

    Memory::~Memory(){
        CloseHandle(handle);
    }

    uintptr_t Memory::getProcess(std::string proc)
    {
        HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        uintptr_t process = 0;
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);

        do
        {
            if (_stricmp(pEntry.szExeFile, proc.c_str()))
            {
                process = pEntry.th32ProcessID;
                CloseHandle(hProcessId);
                handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
            }

        } while (Process32Next(hProcessId, &pEntry));
        return process;
    }

    uintptr_t Memory::getModule(uintptr_t procId, const wchar_t* modName)
    {
        HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
        MODULEENTRY32 mEntry;
        mEntry.dwSize = sizeof(mEntry);

        do
        {
            if (!_wcsicmp((wchar_t*)mEntry.szModule, modName))
            {
                CloseHandle(hModule);
                return (uintptr_t)mEntry.hModule;
            }
        } while (Module32Next(hModule, &mEntry));
        return 0;
    }

    uintptr_t Memory::getAddress(uintptr_t addr, std::vector<uintptr_t> vect)
    {
        for (unsigned int i = 0; i < vect.size(); i++)
        {
            ReadProcessMemory(handle, (BYTE*)addr, &addr, sizeof(addr), 0);
            addr += vect[i];
        }
        return addr;
    }

    bool Memory::memoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) noexcept
    {
        for (; *szMask; ++szMask, ++bData, ++bMask) {
            if (*szMask == 'x' && *bData != *bMask) {
                return false;
            }
        }

        return ((*szMask) == NULL);
    }

    uintptr_t Memory::findSignatureAddress(uintptr_t start, int size, const char* signature, const char* mask) noexcept
    {
        BYTE* data = new BYTE[size];

        ReadProcessMemory(handle, (LPVOID)start, data, size, nullptr);

        for (std::size_t i = 0; i < size; ++i)
        {
            if (memoryCompare((const BYTE*)(data + i), (const BYTE*)(signature), mask)) {
                return start + i;
            }
        }

        delete[] data;

        return NULL;
    }
}
