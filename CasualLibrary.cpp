#include <iostream>

#include "Memory.h"

int main(){
    std::cout << "Testing 1,2,3\n";

    External::Memory memory = External::Memory("mspaint.exe");
    uintptr_t adressToRead = 0x7CBEE536C;
    uintptr_t test = memory.read<uintptr_t>(adressToRead);
    std::cout << std::hex << adressToRead << std::endl;
    std::cin.get();
}
