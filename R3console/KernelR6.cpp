#include <iostream>

#include <stdio.h>

#include "toolkit.cpp"

#include <windows.h>

using namespace std;

int main()
{
	printBaseInfo();
	system("pause");
	int status = 0;
	int statusbefore = 0;
	while (TRUE)
	{
		status = getStatus();
		if ((GetAsyncKeyState(VK_F1) & 0x8000))
			glow(255.0f, 0.0f, 0.0f, 999.0f);
		//if ((GetAsyncKeyState(VK_F3) & 0x8000))
		//	ESP1();
		if ((status == 3)&&(status!=statusbefore))
			ESP2();
		if ((status==3))
			norecoil_temp();
		if ((GetAsyncKeyState(VK_F6) & 0x8000))
			nospread();
		statusbefore = status;
		//if ((GetAsyncKeyState(VK_F7) & 0x8000))
		//	outlineESP_Drone();
		//if ((GetAsyncKeyState(VK_F8) & 0x8000))
		//	outlineESP_EXIT();
	}
	Sleep(50);
}

