#include <iostream>

#include <CasualLibrary.hpp>

inline const char* const BoolToString(bool b) {
    return b ? "OK" : "Failed";
}


int main() {
    std::cout << "Running tests ...\n\n";

    Memory::External external = Memory::External("ZombieWar.exe");
    Address modulePtr = external.getModule("ZombieWar.exe");

    Address namePtr = external.getAddress(modulePtr + 0x00003648, { 0x40 }).get();

    std::cout << "Status external.read<std::string>(addToBeRead):       ";
    std::cout << BoolToString(external.read<std::string>(namePtr) == "CasualGamer") << std::endl;

    //std::cout << "Status readString(addToBeRead, size): ";
    //std::cout << BoolToString(external.readString(namePtr, 5) == "Casua") << std::endl;

    std::cout << "Status external.read<T>(addToBeRead):   ";
    std::cout << BoolToString(external.read<int>(namePtr) == 1970495811) << std::endl;

    Address address = external.findSignature(modulePtr, "BA ? ? ? ? CD", 100);
    std::cout << "Status findSignature(...):            ";
    std::cout << BoolToString(address.get()) << std::endl;

    std::cin.get();
}