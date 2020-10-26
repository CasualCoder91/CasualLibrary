#include <iostream>

#include "Memory.h"

inline const char* const BoolToString(bool b){
    return b ? "OK" : "Failed";
}


int main(){
    std::cout << "Running tests ...\n\n";

    External::Memory memory = External::Memory("ZW64.exe");
    uintptr_t modulePtr = memory.getModule("ZW64.exe");

    uintptr_t namePtr = memory.getAddress(modulePtr + 0x00003648, { 0x40 });

    std::cout << "Status readString(addToBeRead):       ";
    std::cout << BoolToString(memory.readString(namePtr) == "CasualGamer") << std::endl;

    std::cout << "Status readString(addToBeRead, size): ";
    std::cout << BoolToString(memory.readString(namePtr, 5) == "Casua") << std::endl;

    std::cout << "Status memory.read<T>(addToBeRead):   ";
    std::cout << BoolToString(memory.read<int>(namePtr) == 1970495811) << std::endl;

    uintptr_t address = memory.findSignatureAddress(0x7FFA6E0186A0, 1000, { -1, 0x13, -1, 0xC0, -1, 0x8B, 0x5C, 0x24, 0x30 });
    std::cout << "Status findSignatureAddress(...):     ";
    std::cout << BoolToString(address == 0x7ffa6e0188d0) << std::endl;

    std::cin.get();
}
