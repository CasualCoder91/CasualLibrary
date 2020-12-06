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

#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>

#include "../Helper.hpp"
#include "../Address.hpp"
#include "../Macros.hpp"

namespace Memory {
	[[nodiscard]] std::vector<int> patternToBytes(const char* pattern) noexcept;
	[[nodiscard]] std::string getLastErrorAsString(void) noexcept;
    [[nodiscard]] std::string convertToString(char* a, int size) noexcept;

    // Sadly, only C++17 feature because of the folding
#ifdef CPP17GRT
    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool is_any_type(void) noexcept {
        return (std::is_same_v<T, Ts> || ...);
    }
#endif
}