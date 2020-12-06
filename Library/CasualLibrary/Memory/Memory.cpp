#include "Memory.hpp"

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
[[nodiscard]] std::string Memory::getLastErrorAsString(void) noexcept
{
    //Get the error message, if any.
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

[[nodiscard]] std::string Memory::convertToString(char* a, int size) noexcept
{
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

[[nodiscard]] std::vector<int> Memory::patternToBytes(const char* pattern) noexcept 
{
    std::vector<int> bytes{};
    bytes.reserve(strlen(pattern));
            
    std::string patternString(pattern);
            
    char* const start = &patternString[0];
    char* const end = &patternString[strlen(pattern)];

    for (char* current = start; current < end; ++current) {
        if (*current == '?') {
            current++;

            if (*current == '?')
                current++;

            bytes.emplace_back(-1);
        }
        else {
            bytes.emplace_back(static_cast<int>(strtoul(current, &current, 16)));
        }
    }
    return bytes;
}