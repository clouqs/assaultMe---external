#include "proc.h"

DWORD GetProcId(const wchar_t* procName) {
	DWORD procId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {

}

uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets) {

}