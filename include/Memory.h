/**
 * The Memory class is a collection functions used for external memory hacking.
 *
 * @author CasualComunity: Alien, C3x0r, CasualGamer
 * @version 0.2 23.10.2020
*/

#pragma once
#include<Windows.h>
#include<Vector>
#include<string>

namespace External {
    class Memory
    {

    public:
        Memory(std::string proc);

        ~Memory();

        template <typename T>
        T read(uintptr_t addToBeRead){
            T varBuff;
            ReadProcessMemory(handle, (LPBYTE*)addToBeRead, &varBuff, sizeof(varBuff), nullptr);
            return varBuff;
        }

        std::string readString(uintptr_t addToBeRead, size_t size=0);

        template <typename Z>
        Z write(uintptr_t addToWrite, Z valToWrite){
            WriteProcessMemory(handle, (LPBYTE*)addToWrite, &valToWrite, sizeof(valToWrite), nullptr);
            return valToWrite;
        }

        /**@brief returns process id of of process with given name \p proc.*/
        uintptr_t getProcess(std::string proc);
        uintptr_t getModule(uintptr_t procId, const wchar_t* modName);
        uintptr_t getAddress(uintptr_t addr, std::vector<uintptr_t> vect);
        bool memoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) noexcept;
        uintptr_t findSignatureAddress(uintptr_t start, int size, const char* signature, const char* mask) noexcept;
    private:
        HANDLE handle;

    };
}
