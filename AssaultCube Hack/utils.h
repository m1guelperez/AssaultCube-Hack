#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace utils {
	//Get process ID by name
	DWORD getProcID(const wchar_t* procName);

	//Gets the module base address
	uintptr_t getModuleBaseAddress(DWORD procID, const wchar_t* modName);

	//Adds pointer offsets together, starting from our base address to interact with the needed memory address
	uintptr_t calculatePtrAddress(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets, std::string name);

	//Check if the process opened successfully
	void checkIfProcessOpened(HANDLE& hprocess, DWORD& procID);
}