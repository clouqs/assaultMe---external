#include <iostream>
#include <vector>
#include <windows.h>
#include "proc.h"
#include "mem.h"

int main() {
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false;

    const int newValue = 1337; //change here

    DWORD procId = GetProcId(L"ac_client.exe");

    if (procId) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
		moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");
		localPlayerPtr = moduleBase + 0x0017E0A8; //1.3.0.2 ver
            healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xEC });

    }
    else {
		std::cout << "Process not found!" << std::endl;
		return 0;
    }



    DWORD dwExit = 0;

	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {
        {
			if (GetAsyncKeyState(VK_F1) & 1) {
				bHealth = !bHealth;
				if (bHealth) {
					std::cout << "Health: ON" << std::endl;
				}
				else {
					std::cout << "Health: OFF" << std::endl;
				}
			}
			if (GetAsyncKeyState(VK_F2) & 1) {
				bAmmo = !bAmmo;
				if (bAmmo) {
					mem::PatchEx((BYTE*)(moduleBase + 0x140), (BYTE*)&newValue, sizeof(newValue), hProcess);
					std::cout << "Ammo: ON" << std::endl;
				}
				else {
					mem::PatchEx((BYTE*)(moduleBase + 0x140), (BYTE*)&newValue, sizeof(newValue), hProcess);
					std::cout << "Ammo: OFF" << std::endl;
				}
			}
			if (GetAsyncKeyState(VK_F3) & 1) {
				bRecoil = !bRecoil;
				if (bRecoil) {
					mem::NopEx((BYTE*)(moduleBase + ))
					std::cout << "Recoil: ON" << std::endl;
				}
				else {
					std::cout << "Recoil: OFF" << std::endl;
				}
			}



}
    



