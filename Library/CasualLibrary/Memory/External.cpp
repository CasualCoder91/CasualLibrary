#include "External.hpp"

Memory::External::External(const char* proc, const bool debug) noexcept {
    if (!init(proc)) {
        std::cerr << "Could not init Memory object.\n";
        std::cerr << getLastErrorAsString() << std::endl;
    }
    else if (!Helper::matchingBuilt(this->handle)) {
        std::cerr << "x64/x86-bit missmatch! Make sure to match the target." << std::endl;
    };
    this->debug = debug;
}

Memory::External::~External(void) noexcept {
    CloseHandle(handle);
}

[[nodiscard]] std::string Memory::External::readString(const uintptr_t addToBeRead, std::size_t size) noexcept {
    bool oneWord = size == 0;

    if (oneWord)
        size = 200;

    std::vector<char> chars(size);

    if (!ReadProcessMemory(handle, reinterpret_cast<LPBYTE*>(addToBeRead), chars.data(), size, NULL) && debug) {
        std::cout << getLastErrorAsString() << std::endl;
    };

    std::string name(chars.begin(), chars.end());

    if (oneWord)
        return name.substr(0, name.find('\0'));

    return name;
}

bool Memory::External::init(const char* proc, const DWORD access) noexcept {
    HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    this->processID = 0;
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);

    do {
        if (!strcmp(static_cast<char*> (pEntry.szExeFile), proc)) {
            this->processID = pEntry.th32ProcessID;
            CloseHandle(hProcessId);
            this->handle = OpenProcess(access, false, this->processID);
            return true;
        }

    } while (Process32Next(hProcessId, &pEntry));
    if (this->processID == 0)
        CloseHandle(hProcessId);
    return this->processID != 0;
}

[[nodiscard]] DWORD Memory::External::getProcessID(void) noexcept {
    return this->processID;
}

[[nodiscard]] Address Memory::External::getModule(const char* modName) noexcept {
    HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->processID);
    MODULEENTRY32 mEntry;
    mEntry.dwSize = sizeof(mEntry);

    do {
        if (!strcmp(modName, static_cast<char*> (mEntry.szModule))) {
            CloseHandle(hModule);
            return reinterpret_cast<uintptr_t>(mEntry.hModule);
        }
    } while (Module32Next(hModule, &mEntry));
    return nullptr;
}

[[nodiscard]] Address Memory::External::getAddress(uintptr_t addr, const std::vector<uintptr_t>& vect) noexcept {
    for (size_t i = 0; i < vect.size(); i++) {
        if (!ReadProcessMemory(handle, reinterpret_cast<BYTE*>(addr), &addr, sizeof(addr), 0) && debug) {
            std::cout << getLastErrorAsString() << std::endl;
        }
        addr += vect[i];
    }
    return addr;
}

[[nodiscard]] Address Memory::External::getAddress(const Address& addr, const std::vector<uintptr_t>& vect) noexcept {
    return getAddress(addr.get(), vect);
}

[[nodiscard]] bool Memory::External::memoryCompare(const BYTE* bData, const std::vector<int>& signature) noexcept {
    for (size_t i = 0; i < signature.size(); i++) {
        if (signature[i] != -1 && signature[i] != bData[i])
            break;
        if (i + 1 == signature.size())
            return true;
    }
    return false;
}

[[nodiscard]] Address Memory::External::findSignature(const uintptr_t start, const char* signature, const size_t size) noexcept {
    std::vector<int> patternBytes = patternToBytes(signature);
    BYTE* data = new BYTE[size];

    if (!ReadProcessMemory(handle, reinterpret_cast<LPVOID>(start), data, size, nullptr) && debug) {
        std::cout << getLastErrorAsString() << std::endl;
    }

    for (std::size_t i = 0; i < size; ++i) {
        if (memoryCompare(static_cast<const BYTE*>(data + i), patternBytes)) {
            delete[] data;
            return start + i;
        }
    }

    delete[] data;

    return nullptr;
}

[[nodiscard]] Address Memory::External::findSignature(const Address& start, const char* sig, const size_t size) noexcept {
    return findSignature(start.get(), sig, size);
}