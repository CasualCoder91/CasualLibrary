#include <iostream>

#include "Memory.h"

inline const char* const BoolToString(bool b){
    return b ? "OK" : "Failed";
}


int main(){
    std::cout << "Running tests ...\n\n";

    External::Memory memory = External::Memory("ZombieWar.exe");
    Address modulePtr = memory.getModule("ZombieWar.exe");

    Address namePtr = memory.getAddress(modulePtr+0x00003648, { 0x40 }).get();

    std::cout << "Status readString(addToBeRead):       ";
    std::cout << BoolToString(memory.readString(namePtr) == "CasualGamer") << std::endl;

    std::cout << "Status readString(addToBeRead, size): ";
    std::cout << BoolToString(memory.readString(namePtr, 5) == "Casua") << std::endl;

    std::cout << "Status memory.read<T>(addToBeRead):   ";
    std::cout << BoolToString(memory.read<int>(namePtr) == 1970495811) << std::endl;

    uintptr_t address = memory.findSignature(modulePtr, 100, "BA ? ? ? ? CD").get();
    std::cout << "Status findSignature(...):            ";
    std::cout << BoolToString(address) << std::endl;

    // EXAMPLE FOR FINDING SIGNATURE OF D3D9 DEVICE:
     //IDirect3DDevice9* address = **reinterpret_cast<IDirect3DDevice9***>(Internal::Memory::findModuleSignature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C").addOffset(1).get<uint8_t*>());

    std::cin.get();
}