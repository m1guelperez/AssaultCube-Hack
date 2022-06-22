#include <Windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <vector>
#include "utils.h"

namespace utils {
	DWORD getProcID(const wchar_t* procName) {

		DWORD procID = 0;
		PROCESSENTRY32 pe32;

		//Create a snapshot of all processes.
		HANDLE snapshot{};
		snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE) {
			std::cout << "Process-snapshot failed" << std::endl;
			if (snapshot != NULL)
			{
				CloseHandle(snapshot);
			}
		}
		pe32.dwSize = sizeof(pe32);

		if (Process32First(snapshot, &pe32)) {
			do
			{
				//Need ! because it returns 0 if the strings are the same.
				if (!_wcsicmp(pe32.szExeFile, procName)) {
					procID = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &pe32));
		}
		else {
			std::cout << "Process32First failed " << GetLastError() << std::endl;
		}
		CloseHandle(snapshot);
		return procID;
	}


	uintptr_t getModuleBaseAddress(DWORD procID, const wchar_t* modName) {

		MODULEENTRY32 m32;
		uintptr_t baseMod = 0;

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
		if (snapshot == INVALID_HANDLE_VALUE) {
			std::cout << "Module-snapshot failed" << std::endl;
		}
		m32.dwSize = sizeof(m32);

		if (Module32First(snapshot, &m32)) {
			do
			{
				if (!_wcsicmp(m32.szModule, modName))
				{
					baseMod = (uintptr_t)m32.modBaseAddr;
					break;
				}
			} while (Module32Next(snapshot, &m32));
		}
		if (snapshot != NULL)
		{
			CloseHandle(snapshot);
		}
		return baseMod;
	}

	uintptr_t findDMAAddy(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets, std::string name) {
		uintptr_t addr = ptr;
		//iteratively adds the offset to addr and stores it back into addr.
		for (unsigned int i = 0; i < offsets.size(); ++i) {
			ReadProcessMemory(hproc, (BYTE*)addr, &addr, sizeof(addr), nullptr); //sizeof u_intptr should work as well.
			addr += offsets[i];
		}
		std::cout << name << " addr" << " = " << "0x" << std::hex << addr << std::endl;
		return addr;
	}

	void checkIfProcessOpened(HANDLE& hProcess, DWORD& procID) {
		if (hProcess == NULL) {
			std::cout << "Error, could not open process. The following error occurred: " << GetLastError() << std::endl;
			// We do not have any files or similar where we have to ensure everything was written to
			exit(0);
		}
		else {
			std::cout << "Opened process successfully with PID: " << std::dec << procID << std::endl;
		}
	}

	void buttonPressed(int key) {
		bool pressed = false;
		if ((GetAsyncKeyState(VK_BACK) && 0x8000) && !pressed) {
			pressed = true;
		}
		//Check if MSB is 0 (button is not down)
		else if (((GetAsyncKeyState(VK_BACK) && 0x8000) == 0) && pressed)
		{
			pressed = false;
		}
	}
}