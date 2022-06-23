#include "aimbot.h"
#include <Windows.h>
#include <iostream>
#include <stdint.h>

void noRecoil(HANDLE& hProcess, uintptr_t baseAddress[], bool toggleFlag, float& temp) {

	//TODO: This function might be valuable for the aimbot
	/*float currAngleVerRead = 0.0f;
	if (toggleFlag)
	{

		if ((GetAsyncKeyState(VK_LBUTTON) && 0x8000) == 0)
		{
			ReadProcessMemory(hProcess, (BYTE*)baseAddress, &currAngleVerRead, sizeof(float), nullptr);
			temp = currAngleVerRead;
		}
		else if ((GetAsyncKeyState(VK_LBUTTON) && 0x8000))
		{
			WriteProcessMemory(hProcess, (BYTE*)baseAddress, &temp, sizeof(float), nullptr);
		}
	}*/

	//UINT16 since we interact with a 2byte value
	UINT16 setRecoil = 0;
	if (toggleFlag)
	{
		if ((GetAsyncKeyState(VK_LBUTTON) && 0x8000))
		{
			WriteProcessMemory(hProcess, (BYTE*)baseAddress[0], &setRecoil, sizeof(UINT16), nullptr);
			WriteProcessMemory(hProcess, (BYTE*)baseAddress[1], &setRecoil, sizeof(UINT16), nullptr);
		}
	}

}


void aimbot() {

}