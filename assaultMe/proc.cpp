#include "proc.h"

DWORD GetProcId(const wchar_t* procName) { // This function takes a process name as input and returns its process ID.
	DWORD procId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnapshot, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &procEntry));
		}
		CloseHandle(hSnapshot);
	}


}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {

}

uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets) {

}