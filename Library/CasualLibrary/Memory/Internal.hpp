/**
 * Serves as communicator between injected dll and the games memory.
 *
 * @author CasualComunity: tomsa, CasualGamer, Alien
 * @version 0.2 06.11.2020
 * @todo: optimize scanner speed, memory checks
*/

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>

#include "../Helper.hpp"
#include "../Address.hpp"
#include "Memory.hpp"

namespace Memory {
    namespace Internal
    {
        /**
        @brief reads a value from memory
        @param addToBeRead address from which the value will be read.
        */
        template <typename T>
        [[nodiscard]] T read(const Address& addToBeRead) noexcept {
            return *reinterpret_cast<T*>(addToBeRead.get());
        }

        /**
        @brief writes a value to memory
        @param address address to which the value will be written to.
        @param value the value that gets written to desired address.
        */
        template<typename T>
        void write(const uintptr_t address, const T value) noexcept {
            try { 
                *reinterpret_cast<T*>(address) = value;
            } catch (...) {
                return;
            }
        }

        /**@brief returns the module base address of \p modName (example: "client.dll")*/
        [[nodiscard]] Address getModule(const char* modName) noexcept;

        /**@brief returns the dynamic pointer from base pointer \p basePrt and \p offsets*/
        [[nodiscard]] Address getAddress(const uintptr_t basePrt, const std::vector<uintptr_t>& offsets) noexcept;

        /**@brief returns the dynamic pointer from base pointer \p basePrt and \p offsets*/
        [[nodiscard]] Address getAddress(const Address& basePrt, const std::vector<uintptr_t>& offsets) noexcept;

        /**
        @brief a basic signature scanner
        @param start Address where to start scanning.
        @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
        @param size Size of the area to search within.
        */
        [[nodiscard]] Address findSignature(const uintptr_t start, const char* sig, const size_t size = 0) noexcept;
        [[nodiscard]] Address findSignature(const Address& modAddr, const char* sig, const size_t size = 0) noexcept;

        /**
        DO NOT IGNORE: YOU ALWAYS HAVE TO USE THIS FUNCTION (findModuleSignature) WHEN SCANNING FOR SIGNATURES,
        BECAUSE IT RECEIVES THE MODULE BASE AND SIZE OF IMAGE, YOU SHOULD THINK OF IT AS A WRAPPER FOR THE findSignature FUNCTION.
        @brief a basic signature scanner searching within the address space of one module.
        @param mod name of the module ("client.dll")
        @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
        */
        [[nodiscard]] Address findModuleSignature(const char* mod, const char* sig) noexcept;

        /**
         @brief gets a virtual function via vtable index, don't use this, use callVirtualFunction,
         the only reason this is here is because its wrapped around callVirtualFunction.
         @param baseClass vtable type
         @param index index in vtable
         */
        template<typename T>
        [[nodiscard]] T getVirtualFunction(const void* baseClass, const uint32_t index) noexcept {
            return (*static_cast<T**>(baseClass))[index];
        }

        /**
        @brief calls a virtual function
        @param baseClass vtable type
        @param index index in vtable
        @param args the function's parameters
        */
        template<typename T, typename... Args>
        T callVirtualFunction(const void* baseClass, const uint32_t index, Args&&... args) noexcept {
            return getVirtualFunction<T(__thiscall*)(void*, Args&&...)>(baseClass, index)(baseClass, std::forward<Args>(args)...);
        }
    }
}