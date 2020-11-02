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

    uintptr_t address = memory.findSignatureAddress(modulePtr, 100, { 0xBA, -1, -1, -1, -1, 0xCD });
    std::cout << "Status findSignatureAddress(...):     ";
    std::cout << BoolToString(address) << std::endl;

    // EXAMPLE FOR FINDING SIGNATURE OF D3D9 DEVICE:
    // uint8_t* address = **reinterpret_cast<IDirect3DDevice9***>(Internal::Memory::findModuleSignature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C").addOffset(1).get<uint8_t*>());

    std::cin.get();
}