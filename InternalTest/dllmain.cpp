// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>

#include <Memory.h>

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
    std::cout << "Running tests ...\n\n";

    Address modulePtr = Internal::Memory::getModule("ZombieWar.exe");

    Address namePtr = Internal::Memory::getAddress(modulePtr + 0x00003648, { 0x40 }).get();

    /*std::cout << "Status readString(addToBeRead):       ";
    std::cout << BoolToString(Internal::Memory::readString(namePtr) == "CasualGamer") << std::endl;

    std::cout << "Status readString(addToBeRead, size): ";
    std::cout << BoolToString(memory.readString(namePtr, 5) == "Casua") << std::endl;*/

    std::cout << "Status memory.read<T>(addToBeRead):   ";
    std::cout << BoolToString(Internal::Memory::read<int>(namePtr) == 1970495811) << std::endl;

    uintptr_t address = Internal::Memory::findSignature(modulePtr, "BA ? ? ? ? CD", 100).get();
    std::cout << "Status findSignature(...):            ";
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