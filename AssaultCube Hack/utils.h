#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace utils {
	DWORD getProcID(const wchar_t* procName);
	uintptr_t getModuleBaseAddress(DWORD procID, const wchar_t* modName);
	uintptr_t findDMAAddy(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets, std::string name);
	void checkIfProcessOpened(HANDLE& hprocess, DWORD& procID);
}