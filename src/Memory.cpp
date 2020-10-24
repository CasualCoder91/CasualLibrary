#include "Memory.h"

namespace External {

    Memory::Memory(std::string proc) {
        if (!init(proc)) {
            throw "Could not init Memory object.";
        };
    }

    Memory::~Memory(){
        CloseHandle(handle);
    }

    std::string Memory::readString(uintptr_t addToBeRead, size_t size){
        bool oneWord = size == 0;
        if(oneWord)
            size = 200;
        std::vector<char> chars(size);
        ReadProcessMemory(handle, (LPBYTE*)addToBeRead, chars.data(), size, NULL);
        std::string name(chars.begin(), chars.end());
        if (oneWord)
            return name.substr(0, name.find(" "));
        return name;
    }

    bool Memory::init(std::string proc){
        HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        this->processID = 0;
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);

        do{
            if (!_stricmp(pEntry.szExeFile, proc.c_str())){
                this->processID = pEntry.th32ProcessID;
                CloseHandle(hProcessId);
                this->handle = OpenProcess(PROCESS_ALL_ACCESS, false, this->processID);
            }

        } while (Process32Next(hProcessId, &pEntry));
        CloseHandle(hProcessId);
        return this->processID !=0;
    }

    DWORD Memory::getProcessID(){
        return this->processID;
    }

    uintptr_t Memory::getModule(const wchar_t* modName){
        HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->processID);
        MODULEENTRY32 mEntry;
        mEntry.dwSize = sizeof(mEntry);

        do{
            if (!_wcsicmp((wchar_t*)mEntry.szModule, modName)){
                CloseHandle(hModule);
                return (uintptr_t)mEntry.hModule;
            }
        } while (Module32Next(hModule, &mEntry));
        return 0;
    }

    uintptr_t Memory::getAddress(uintptr_t addr, std::vector<uintptr_t> vect){
        for (unsigned int i = 0; i < vect.size(); i++){
            ReadProcessMemory(handle, (BYTE*)addr, &addr, sizeof(addr), 0);
            addr += vect[i];
        }
        return addr;
    }

    bool Memory::memoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) noexcept{
        for (; *szMask; ++szMask, ++bData, ++bMask) {
            if (*szMask == 'x' && *bData != *bMask) {
                return false;
            }
        }

        return ((*szMask) == NULL);
    }

    uintptr_t Memory::findSignatureAddress(uintptr_t start, size_t size, const char* signature, const char* mask) noexcept{
        BYTE* data = new BYTE[size];

        ReadProcessMemory(handle, (LPVOID)start, data, size, nullptr);

        for (std::size_t i = 0; i < size; ++i){
            if (memoryCompare((const BYTE*)(data + i), (const BYTE*)(signature), mask)) {
                return start + i;
            }
        }

        delete[] data;

        return NULL;
    }
}
