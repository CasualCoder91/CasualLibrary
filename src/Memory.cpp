#include "Memory.h"

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

namespace External {

    Memory::Memory(const std::string& proc) {
        if (!init(proc)) {
            throw "Could not init Memory object.";
        };
    }

    Memory::~Memory(void) noexcept {
        CloseHandle(handle);
    }

    std::string Memory::readString(const uintptr_t addToBeRead, std::size_t size) noexcept {
        bool oneWord = size == 0;

        if(oneWord)
            size = 200;

        std::vector<char> chars(size);
        
        if (!ReadProcessMemory(handle, (LPBYTE*)addToBeRead, chars.data(), size, NULL)) {
            std::cout << GetLastErrorAsString() << std::endl;
        }

        std::string name(chars.begin(), chars.end());

        if (oneWord)
            return name.substr(0, name.find('\0'));

        return name;
    }

    bool Memory::init(const std::string& proc, const DWORD access) {
        HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        this->processID = 0;
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);

        do{
            if (!_stricmp(pEntry.szExeFile, proc.c_str())){
                this->processID = pEntry.th32ProcessID;
                CloseHandle(hProcessId);
                this->handle = OpenProcess(access, false, this->processID);
            }

        } while (Process32Next(hProcessId, &pEntry));
        if(this->processID==0)
            CloseHandle(hProcessId);
        return this->processID != 0;
    }

    DWORD Memory::getProcessID() noexcept {
        return this->processID;
    }

    uintptr_t Memory::getModule(const std::string& modName) noexcept {
        HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->processID);
        MODULEENTRY32 mEntry;
        mEntry.dwSize = sizeof(mEntry);

        do{
            if (!_stricmp(mEntry.szModule, modName.c_str())){
                CloseHandle(hModule);
                return (uintptr_t)(mEntry.hModule);
            }
        } while (Module32Next(hModule, &mEntry));
        return 0;
    }

    uintptr_t Memory::getAddress(uintptr_t addr, const std::vector<uintptr_t>& vect) noexcept {
        for (size_t i = 0; i < vect.size(); i++){
            if (!ReadProcessMemory(handle, (BYTE*)(addr), &addr, sizeof(addr), 0)) {
                std::cout << GetLastErrorAsString() << std::endl;
            }
            addr += vect[i];
        }
        return addr;
    }

    bool Memory::memoryCompare(const BYTE* bData, const std::vector<int>& signature) noexcept {
        for (size_t i = 0; i < signature.size(); i++) {
            if (signature[i] != -1 && signature[i] != bData[i])
                break;
            if (i + 1 == signature.size())
                return true;
        }
        return false;
    }

    uintptr_t Memory::findSignatureAddress(const uintptr_t start, const size_t size, const std::vector<int>& signature) noexcept {
        BYTE* data = new BYTE[size];

        if (!ReadProcessMemory(handle, (LPVOID)(start), data, size, nullptr)) {
            std::cout << GetLastErrorAsString() << std::endl;
        }

        for (std::size_t i = 0; i < size; ++i){
            if (memoryCompare((const BYTE*)(data + i), signature)) {
                delete[] data;
                return start + i;
            }
        }

        delete[] data;

        return NULL;
    }
}
