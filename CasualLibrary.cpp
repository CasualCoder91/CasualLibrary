#include <iostream>

#include "Memory.h"

int main(){
    std::cout << "Testing 1,2,3\n";

    External::Memory memory = External::Memory("mspaint.exe");
    uintptr_t adressToRead = 0x2240001C;
    uintptr_t test = memory.read<uintptr_t>(adressToRead);
    std::cout << std::hex << adressToRead << std::endl; 
    
    uintptr_t modulePtr = memory.getModule("mspaint.exe");
    uintptr_t namePtr = modulePtr + 0xDD075;
    uintptr_t intPtr = modulePtr + 0xDD5C0;

    std::string word = memory.readString(namePtr);
    std::string text = memory.readString(namePtr, 200);
    int number = memory.read<int>(intPtr);

    std::cout << "Word at location: " << word << std::endl;
    std::cout << "String at location: " << text << std::endl;
    std::cout << "Int at location: " << number << std::endl;

    uintptr_t address = memory.findSignatureAddress(0x7FF702CB5D00, 10000, { -1, 0x39, 0x05, 0xF0, 0xA2, 0xF6, 0xFF });
    std::cout << "Scanned Location: " << std::hex << address << std::endl;
    std::cin.get();
}
