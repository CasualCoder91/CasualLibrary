/**
 * The Memory class is a collection functions used for external memory hacking.
 *
 * @author CasualCommunity: Alien, C3x0r, CasualGamer
 * @version 0.1 23.10.2020
*/

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <vector>
#include <string>
#include <TlHelp32.h>
#include <iostream>

#include "Helper.h"

#ifdef _MSC_VER
#pragma warning(disable : 6001)
#endif

namespace External {
    /**
     * Serves as communicator between your application and the games memory.
     *
     * @author CasualComunity: Alien, C3x0r, CasualGamer
     * @version 0.1 23.10.2020
     * @todo: optimize scanner speed, optional parameter with desiredAccess
    */
    class Memory
    {

    public:
        Memory(const std::string& proc);
            
        ~Memory(void) noexcept;

        template <typename T>
        __forceinline T read(const uintptr_t addToBeRead) noexcept {
            T varBuff;
            ReadProcessMemory(handle, (LPBYTE*)addToBeRead, &varBuff, sizeof(varBuff), nullptr);
            return varBuff;
        }

        std::string readString(const uintptr_t addToBeRead, std::size_t size = 0) noexcept;

        template <typename T>
        __forceinline T write(const uintptr_t addToWrite, const T valToWrite) noexcept {
            WriteProcessMemory(handle, (LPBYTE*)addToWrite, &valToWrite, sizeof(valToWrite), nullptr);
            return valToWrite;
        }

        /**@brief returns process id of the target process*/
        DWORD getProcessID(void) noexcept;
        /**@brief returns the module base address of \p modName (example: "client.dll")*/
        uintptr_t getModule(const std::string& modName) noexcept;
        /**@brief returns the dynamic pointer from base pointer \p basePrt and \p offsets*/
        uintptr_t getAddress(const uintptr_t basePrt, const std::vector<uintptr_t>& offsets) noexcept;
        bool memoryCompare(const BYTE* bData, const std::vector<int>& signature) noexcept;
        /**
        @brief a basic signature scanner
        @param start Address where to start scanning.
        @param size Size of the area to search within.
        @param signature Signature to search for. example: {-1, 0x39, 0x05, 0xF0, 0xA2, 0xF6, 0xFF} where -1 is a wild card.
        */
        uintptr_t findSignatureAddress(const uintptr_t start, const size_t size, const std::vector<int>& signature) noexcept;

    private:
        /** @brief handle of the target process */
        HANDLE handle;
        /** @brief ID of the target process */
        DWORD processID = 0;
        /**@brief initialize member variables.
        @param proc Name of the target process ("target.exe")
        @param access Desired access right to the process
        @returns whether or not initialization was successful*/
        bool init(const std::string& proc, const DWORD access = PROCESS_ALL_ACCESS);

    };
}
