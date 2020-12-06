// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>

#include <CasualLibrary.hpp>

HMODULE myhModule;

inline const char* const BoolToString(bool b) {
    return b ? "OK" : "Failed";
}

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(myhModule, 0);
}

DWORD WINAPI Menue() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); // output only
    std::cout << "Running internal tests ...\n\n";

    Address modulePtr = Memory::Internal::getModule("ZombieWar.exe");

    Address namePtr = Memory::Internal::getAddress(modulePtr + 0x00003648, { 0x40 }).get();

    std::cout << "Status Internal::read<std::string>:       ";
    std::cout << BoolToString(Memory::Internal::read<std::string>(namePtr) == "CasualGamer") << std::endl;

    /*std::cout << "Status readString(addToBeRead, size): ";
    std::cout << BoolToString(memory.readString(namePtr, 5) == "Casua") << std::endl;*/

    std::cout << "Status Internal::read<T>(addToBeRead):   ";
    std::cout << BoolToString(Memory::Internal::read<int>(namePtr) == 1970495811) << std::endl;

    std::cout << "Status Internal::read<T>(addToBeRead,true):   ";
    std::cout << BoolToString(Memory::Internal::read<int>(0x7FF6C64A9014, true)==0) << std::endl; //try reading inside PAGE_NOACCESS block

    uintptr_t address = Memory::Internal::findSignature(modulePtr, "BA ? ? ? ? CD", 100).get();
    std::cout << "Status Internal::findSignature(...):            ";
    std::cout << BoolToString(address) << std::endl;
    while (1) {
        Sleep(100);
        if (GetAsyncKeyState(VK_NUMPAD0))
            break;
    }
    fclose(fp);
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);

    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        myhModule = hModule;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Menue, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}