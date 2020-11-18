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
#include <Address.h>

std::vector<int> patternToBytes(const char* pattern);

std::string getLastErrorAsString();

namespace External {
    /**
     * Serves as communicator between your application and the games memory.
     *
     * @author CasualComunity: Alien, C3x0r, CasualGamer
     * @version 0.1.2 06.11.2020
     * @todo: optimize scanner speed, optional parameter with desiredAccess, memory checks, add readString(...) to internal
    */
    class Memory
    {

    public:
        Memory(const char* proc,bool debug = false);
            
        ~Memory(void) noexcept;
        
        /**
        @brief reads a value from memory
        @param addToBeRead address from which the value will be read.
        */
        template <typename T>
        T read(const uintptr_t addToBeRead) noexcept {
            T varBuff;
            ReadProcessMemory(handle, reinterpret_cast<LPBYTE*>(addToBeRead), &varBuff, sizeof(varBuff), nullptr);
            return varBuff;
        }

        template <typename T>
        T read(const Address addToBeRead) noexcept {
            T varBuff;
            ReadProcessMemory(handle, reinterpret_cast<LPBYTE*>(addToBeRead.get()), &varBuff, sizeof(varBuff), nullptr);
            return varBuff;
        }

        /**
        @brief writes a value to memory
        @param addToWrite address to which the value will be written to.
        @param valToWrite the value that gets written to desired address.
        */
        template <typename T>
        T write(const uintptr_t addToWrite, const T valToWrite) noexcept {
            WriteProcessMemory(handle, reinterpret_cast<LPBYTE*>(addToWrite), &valToWrite, sizeof(valToWrite), nullptr);
            return valToWrite;
        }

        std::string readString(const uintptr_t addToBeRead, std::size_t size = 0) noexcept;
        std::string readString(const Address addToBeRead, std::size_t size = 0) noexcept;
        /**@brief returns process id of the target process*/
        DWORD getProcessID(void) noexcept;
        /**@brief returns the module base address of \p modName (example: "client.dll")*/
        Address getModule(const char* modName) noexcept;
        /**@brief returns the dynamic pointer from base pointer \p basePrt and \p offsets*/
        Address getAddress(const uintptr_t basePrt, const std::vector<uintptr_t>& offsets) noexcept;
        /**@brief returns the dynamic pointer from base pointer \p basePrt and \p offsets*/
        Address getAddress(Address basePrt, const std::vector<uintptr_t>& offsets) noexcept;
        bool memoryCompare(const BYTE* bData, const std::vector<int>& signature) noexcept;
        /**
        @brief a basic signature scanner
        @param start Address where to start scanning.
        @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
        @param size Size of the area to search within.
        */
        Address findSignature(const uintptr_t start, const char* sig, const size_t size) noexcept;
        Address findSignature(const Address start, const char* sig, const size_t size) noexcept;

    private:
        /** @brief handle of the target process */
        HANDLE handle = nullptr;
        /** @brief ID of the target process */
        DWORD processID = 0;
        /** @brief debug flag. Set true for debug messages*/
        bool debug = false;
        /**@brief initialize member variables.
        @param proc Name of the target process ("target.exe")
        @param access Desired access right to the process
        @returns whether or not initialization was successful*/
        bool init(const char* proc, const DWORD access = PROCESS_ALL_ACCESS);

    };
}

namespace Internal
{
    /**
     * Serves as communicator between injected dll and the games memory.
     *
     * @author CasualComunity: tomsa, CasualGamer
     * @version 0.2 06.11.2020
     * @todo: optimize scanner speed, memory checks
    */
    namespace Memory
    {
        /**
         @brief reads a value from memory
         @param address address from which the value will be read.
         */
        template<typename T> 
        T read(uintptr_t address) {
            return *reinterpret_cast<T*>(address);
        }

        /**
        @brief reads a value from memory
        @param addToBeRead address from which the value will be read.
        */
        template <typename T>
        T read(const Address addToBeRead) noexcept {
            return *reinterpret_cast<T*>(addToBeRead.get ( ));
        }

        /**
        @brief writes a value to memory
        @param address address to which the value will be written to.
        @param value the value that gets written to desired address.
        */
        template<typename T> 
        void write(uintptr_t address, T value) {
            try { *reinterpret_cast<T*>(address) = value; }
            catch (...) { return; }
        }

        /**@brief returns the module base address of \p modName (example: "client.dll")*/
        Address getModule(const char* modName) noexcept;
        /**@brief returns the dynamic pointer from base pointer \p basePrt and \p offsets*/
        Address getAddress(const uintptr_t basePrt, const std::vector<uintptr_t>& offsets) noexcept;
        /**@brief returns the dynamic pointer from base pointer \p basePrt and \p offsets*/
        Address getAddress(Address basePrt, const std::vector<uintptr_t>& offsets) noexcept;
        /**
        @brief a basic signature scanner
        @param start Address where to start scanning.
        @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
        @param size Size of the area to search within.
        */
        Address findSignature(uintptr_t start, const char* sig, size_t size = 0) noexcept;
        Address findSignature(const Address start, const char* sig, size_t size = 0) noexcept;

        /**
        DO NOT IGNORE: YOU ALWAYS HAVE TO USE THIS FUNCTION (findModuleSignature) WHEN SCANNING FOR SIGNATURES,
        BECAUSE IT RECEIVES THE MODULE BASE AND SIZE OF IMAGE, YOU SHOULD THINK OF IT AS A WRAPPER FOR THE findSignature FUNCTION.
        @brief a basic signature scanner searching within the address space of one module.
        @param mod name of the module ("client.dll")
        @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
        */
        Address findModuleSignature(const char* mod, const char* sig) noexcept;


        /**
         @brief gets a virtual function via vtable index, don't use this, use callVirtualFunction,
         the only reason this is here is because its wrapped around callVirtualFunction.
         @param baseClass vtable type
         @param index index in vtable
         */
        template<typename T>
        T getVirtualFunction(void* baseClass, uint32_t index){
            return (*static_cast<T**>(baseClass))[index];
        }

        /**
        @brief calls a virtual function
        @param baseClass vtable type
        @param index index in vtable
        @param args the function's parameters
        */
        template<typename T, typename ... Args>
        T callVirtualFunction(void* baseClass, uint32_t index, Args... args){
            return getVirtualFunction<T(__thiscall*)(void*, Args...)>(baseClass, index)(baseClass, args...);
        }
    }
}
