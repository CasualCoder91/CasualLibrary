#include <iostream>

#include "Memory.h"

int main(){
    std::cout << "Testing 1,2,3\n";

    External::Memory memory = External::Memory("mspaint.exe");
    uintptr_t adressToRead = 0x2240001C;
    uintptr_t test = memory.read<uintptr_t>(adressToRead);
    std::string word = memory.readString(adressToRead);
    std::string text = memory.readString(adressToRead, 200);
    std::cout << std::hex << adressToRead << std::endl;
    std::cout <<"Word at location: " << word << std::endl;
    std::cout <<"String at location: " << text << std::endl;
    std::cin.get();
}
