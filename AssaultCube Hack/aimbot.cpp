#include "aimbot.h"
#include <Windows.h>
#include <iostream>

void noRecoil(HANDLE& hProcess, uintptr_t baseAddress, bool toggleFlag, float& temp) {

	float currAngleVerRead = 0.0f;
	
	if ((GetAsyncKeyState(VK_LBUTTON) && 0x8000) == 0)
	{
		ReadProcessMemory(hProcess, (BYTE*)baseAddress, &currAngleVerRead, sizeof(float), nullptr);
		temp = currAngleVerRead;
	}
	else if ((GetAsyncKeyState(VK_LBUTTON) && 0x8000))
	{
		WriteProcessMemory(hProcess, (BYTE*)baseAddress, &temp, sizeof(float), nullptr);
	}
}


void aimbot() {

}