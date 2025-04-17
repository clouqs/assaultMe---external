#include <iostream>
#include <vector>
#include <windows.h>
#include "proc.h"

int main()
{
    // Get ProcID of the target process
    DWORD procId = GetProcId(L"ac_client.exe");
    if (procId == 0) {
        std::cerr << "Failed to find process!" << std::endl;
        return 1;
    }

    // Get module base address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    // Get Handle to process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process! Error: " << GetLastError() << std::endl;
        return 1;
    }

    // Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0017D848;
    std::cout << "DynamicPtrBaseAddr = 0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    // Resolve our ammo pointer chain
    std::vector<unsigned int> ammoOffset = { 0x30, 0x58, 0x1F4, 0x18,0x1DC, 0x14, 0x14 }; // Corrected offsets
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffset);
    std::cout << "AmmoAddr = 0x" << std::hex << ammoAddr << std::endl;

    if (ammoAddr == 0) {
        std::cerr << "Failed to find ammo address!" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Read ammo value
    int ammoValue = 0;
    if (!ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr)) {
        std::cerr << "Read failed! Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return 1;
    }
    std::cout << "Current Ammo Value = " << std::dec << ammoValue << std::endl;

    // Update ammo value if <= 1337
    if (ammoValue <= 1337) {
        ammoValue = 1337; // Set to 1337 instead of incrementing
        if (!WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr)) {
            std::cerr << "Write failed! Error: " << GetLastError() << std::endl;
        }
    }

    // Read again to confirm
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "New Ammo Value = " << std::dec << ammoValue << std::endl;

    CloseHandle(hProcess);
    getchar();
    return 0;
}