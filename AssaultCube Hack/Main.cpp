#include <Windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include "utils.h"
#include "aimbot.h"

int main() {

	//Offsets as vectors
	std::vector<unsigned int> currWeaponAmmo = { 0x384,0x14,0x0 };
	std::vector<unsigned int> currWeaponReserveAmmo = { 0x384,0x10,0x0 };
	std::vector<unsigned int> health = { 0xF8 };
	std::vector<unsigned int> grenades = { 0x368,0x14,0x0 };
	std::vector<unsigned int> viewAngleHor = { 0x40 };
	std::vector<unsigned int> viewAngleVer = { 0x44 };




	//Get procID
	DWORD procID = utils::getProcID(L"ac_client.exe");

	//Get Modulebaseaddress
	uintptr_t moduleBase = utils::getModuleBaseAddress(procID, L"ac_client.exe");
	const uintptr_t basePtr = moduleBase + 0x10F4F4;
	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << basePtr << std::endl;

	//Get process handle
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	utils::checkIfProcessOpened(hProcess, procID);

	//Resolve pointer chain
	uintptr_t ammoPtr = utils::findDMAAddy(hProcess, basePtr, currWeaponAmmo, "Current Weapon Ammo");
	uintptr_t reserveAmmoPtr = utils::findDMAAddy(hProcess, basePtr, currWeaponReserveAmmo, "Current weapon reserve Ammo");
	uintptr_t healthPtr = utils::findDMAAddy(hProcess, basePtr, health, "Health");
	uintptr_t grenadePtr = utils::findDMAAddy(hProcess, basePtr, grenades, "Grenades");
	uintptr_t viewAngleVerPtr = utils::findDMAAddy(hProcess, basePtr, viewAngleVer, "ViewAngle Vertical");

	int ammoRead = 0;
	int ammoTemp = 0;
	const int setAmmo = 33;
	int reserveAmmoRead = 0;
	const int setReserveAmmo = 123;
	int healthRead = 100;
	const int setHealth = 987;
	int grenadesRead = 0;
	const int setGrenades = 10;

	bool pressed = false;
	bool activated = false;
	bool aimbotFlag = true;
	float temp = 0.0f;

	while (true)
	{
		//Check if MSB is 1 (button is down)
		if ((GetAsyncKeyState(VK_BACK) && 0x8000) && !pressed) {
			pressed = true;
			if (activated)
			{
				activated = false;
				std::cout << "Cheat deactivated" << std::endl;
			}
			else {
				activated = true;
				std::cout << "Cheat activated" << std::endl;
			}
		}
		//Check if MSB is 0 (button is not down)
		else if (((GetAsyncKeyState(VK_BACK) && 0x8000) == 0) && pressed)
		{
			pressed = false;
		}

		if (activated)
		{
			ReadProcessMemory(hProcess, (BYTE*)ammoPtr, &ammoRead, sizeof(int), nullptr);
			ReadProcessMemory(hProcess, (BYTE*)reserveAmmoPtr, &reserveAmmoRead, sizeof(int), nullptr);
			ReadProcessMemory(hProcess, (BYTE*)healthPtr, &healthRead, sizeof(int), nullptr);
			ReadProcessMemory(hProcess, (BYTE*)grenadePtr, &grenadesRead, sizeof(int), nullptr);

			//TODO: Overhaul
			noRecoil(hProcess, viewAngleVerPtr, aimbotFlag,temp);
			if (ammoRead != ammoTemp)
			{
				std::cout << "Ammo: " << std::dec << ammoRead << std::endl;
				ammoTemp = ammoRead;
			}
			if (ammoRead == 5)
			{
				WriteProcessMemory(hProcess, (BYTE*)ammoPtr, &setAmmo, sizeof(int), nullptr);
			}
			if (healthRead < 100)
			{
				WriteProcessMemory(hProcess, (BYTE*)healthPtr, &setHealth, sizeof(int), nullptr);
				std::cout << std::dec << "Health: " << healthRead << std::endl;

			}
			if (grenadesRead < 2)
			{
				WriteProcessMemory(hProcess, (BYTE*)grenadePtr, &setGrenades, sizeof(int), nullptr);
				std::cout << std::dec << "Grenades: " << grenadesRead << std::endl;
			}
			if (reserveAmmoPtr < 10)
			{
				WriteProcessMemory(hProcess, (BYTE*)reserveAmmoPtr, &setReserveAmmo, sizeof(int), nullptr);
				std::cout << std::dec << "ReserveAmmo: " << reserveAmmoRead << std::endl;
			}
		}

	}
	CloseHandle(hProcess);
	return 0;
}