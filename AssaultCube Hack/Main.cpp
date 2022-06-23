#include <Windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include "utils.h"
#include "aimbot.h"
#include "player.h"

int main() {

	//Offsets as vectors
	std::vector<unsigned int> currWeaponAmmo = { 0x374,0x14,0x0 };
	std::vector<unsigned int> currWeaponReserveAmmo = { 0x384,0x10,0x0 };
	std::vector<unsigned int> health = { 0xF8 };
	std::vector<unsigned int> grenades = { 0x368,0x14,0x0 };
	std::vector<unsigned int> viewAngleHor = { 0x40 };
	std::vector<unsigned int> viewAngleVer = { 0x44 };
	std::vector<unsigned int> recoil1 = { 0x374, 0xC, 0x120 };
	std::vector<unsigned int> recoil2 = { 0x374, 0xC, 0x122 };

	//TODO: Make all individual hack functions toggable
	player::Player player{};
	player.noRecoilToggle = true;




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
	uintptr_t ammoPtr = utils::calculatePtrAddress(hProcess, basePtr, currWeaponAmmo, "Current Weapon Ammo");
	uintptr_t reserveAmmoPtr = utils::calculatePtrAddress(hProcess, basePtr, currWeaponReserveAmmo, "Current weapon reserve Ammo");
	uintptr_t healthPtr = utils::calculatePtrAddress(hProcess, basePtr, health, "Health");
	uintptr_t grenadePtr = utils::calculatePtrAddress(hProcess, basePtr, grenades, "Grenades");
	uintptr_t viewAngleVerPtr = utils::calculatePtrAddress(hProcess, basePtr, viewAngleVer, "ViewAngle Vertical");
	//Need to recoil values. One is for the initial recoil and the second for sustained fire
	uintptr_t recoilPtr1 = utils::calculatePtrAddress(hProcess, basePtr, recoil1, "Recoil1");
	uintptr_t recoilPtr2 = utils::calculatePtrAddress(hProcess, basePtr, recoil2, "Recoil2");
	uintptr_t recoilArr[2] = {recoilPtr1,recoilPtr2};

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
	float temp = 0.0f;

	//TODO: Refactor the loop and add more key-shortcut support.
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

		/*if ((GetAsyncKeyState(VK_CONTROL) && 0x8000) && (GetAsyncKeyState(0x4F) && 0x8000) && !pressed) {
			pressed = true;
			std::cout << "here" << std::endl;
			if (!player.noRecoilToggle)
			{
				player.noRecoilToggle = true;
				std::cout << "NoRecoil activated" << std::endl;
			}
			else {
				player.noRecoilToggle = false;
				std::cout << "NoRecoil deactivated" << std::endl;
			}
		}
		else if (((GetAsyncKeyState(VK_CONTROL) && 0x8000) == 0 && (GetAsyncKeyState(0x4F) && 0x8000) == 0) && pressed)
		{
			std::cout << "here lol" << std::endl;
			pressed = false;
		}*/


		if (activated)
		{
			ReadProcessMemory(hProcess, (BYTE*)ammoPtr, &ammoRead, sizeof(int), nullptr);
			ReadProcessMemory(hProcess, (BYTE*)reserveAmmoPtr, &reserveAmmoRead, sizeof(int), nullptr);
			ReadProcessMemory(hProcess, (BYTE*)healthPtr, &healthRead, sizeof(int), nullptr);
			ReadProcessMemory(hProcess, (BYTE*)grenadePtr, &grenadesRead, sizeof(int), nullptr);

			//TODO: Overhaul
			noRecoil(hProcess, recoilArr, true, temp);
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