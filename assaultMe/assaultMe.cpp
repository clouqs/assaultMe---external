#include <iostream>
#include <windows.h>
#include "proc.h"
#include "mem.h"

int main()
{
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false;

    const int newValue = 1337; // Health value to write

    // Get Process ID
    DWORD procId = GetProcId(L"ac_client.exe");

    if (procId)
    {
        // Open process handle
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

        // Get module base address
        moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

        // Resolve local player pointer
        localPlayerPtr = moduleBase + 0x0017E0A8;

        // Resolve health address (localPlayerPtr + 0xEC)
        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xEC });

        std::cout << "Process found! PID: " << procId << std::endl;
    }
    else
    {
        std::cout << "Process not found. Press Enter to exit." << std::endl;
        getchar();
        return 0;
    }

    DWORD dwExit = 0;
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        
        if (GetAsyncKeyState(VK_F1) & 1) //needs update
        {
            bHealth = !bHealth;
            std::cout << "Health Hack: " << (bHealth ? "ON" : "OFF") << std::endl;
        }

        
        if (GetAsyncKeyState(VK_F2) & 1) //needs fix
        {
            bAmmo = !bAmmo;

            if (bAmmo)
            {
                
                mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x06", 2, hProcess);
                std::cout << "Unlimited Ammo: ON" << std::endl;
            }
            else
            {
                
                mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2, hProcess);
                std::cout << "Unlimited Ammo: OFF" << std::endl;
            }
        }

        
        if (GetAsyncKeyState(VK_F3) & 1) //fixed!!
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                
                mem::NopEx((BYTE*)(moduleBase + 0xC2EC3), 5, hProcess);
                std::cout << "No Recoil: ON" << std::endl;
            }
            else
            {
                
                mem::PatchEx((BYTE*)(moduleBase + 0xC2EC3), (BYTE*)"\xF3\x0F\x11\x56\x38", 5, hProcess);
                std::cout << "No Recoil: OFF" << std::endl;
            }
        }

        // Exit Hack (INSERT)
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            std::cout << "Exiting..." << std::endl;
            return 0;
        }

        // Continuous Health Write
        if (bHealth)
        {
            mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }

        Sleep(10); 
    }

    std::cout << "Process closed. Press Enter to exit." << std::endl;
    getchar();
    return 0;
}