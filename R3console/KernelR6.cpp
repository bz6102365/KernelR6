#include <iostream>

#include "KeInterface64.h"

#include <stdio.h>

#include "Offsets.h"

#include <windows.h>

using namespace std;

KeInterface Driver("\\\\.\\linkdriver1");
DWORD64 ProcessId;
DWORD64 ClientAddress;

void glow(float r, float g, float b, float a)
{
	DWORD64 glowManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_GLOW_MANAGER, sizeof(DWORD64));
	DWORD64 glowBase = Driver.ReadVirtualMemory<DWORD64>(ProcessId, glowManager + 0xB8, sizeof(DWORD64));
	std::cout << "Found glowManager at 0x" << glowManager << std::endl;
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x110, *(PULONG64)(&r), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x114, *(PULONG64)(&g), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x118, *(PULONG64)(&b), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, glowBase + 0x130, *(PULONG64)(&a), sizeof(float));
	printf("glowed!\n");
	//Sleep(500);
}

WORD max_entities()
{
	DWORD64 gameManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_GAME_MANAGER, sizeof(DWORD64));
	return Driver.ReadVirtualMemory<WORD>(ProcessId, gameManager + 0x1C8 + 0x8, sizeof(WORD));
}

DWORD64 entity(DWORD64 index)
{
	DWORD64 gameManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_GAME_MANAGER, sizeof(DWORD64));
	DWORD64 entity_list = Driver.ReadVirtualMemory<DWORD64>(ProcessId, gameManager + 0x1C8, sizeof(DWORD64));
	return Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity_list + index*sizeof(std::uintptr_t), sizeof(std::uintptr_t));
}

void enable_marker(DWORD64 entity)
{
	DWORD64 entity_info = Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity + 0x28, sizeof(DWORD64));
	DWORD64 component_list = Driver.ReadVirtualMemory<DWORD64>(ProcessId, entity_info + 0xD8, sizeof(DWORD64));
	for (DWORD64 current_component = 0x80; current_component < 0xf0; current_component += sizeof(std::uintptr_t))
	{
		DWORD64 component = Driver.ReadVirtualMemory<DWORD64>(ProcessId, component_list + current_component, sizeof(DWORD64));
		if (!component) {
			continue;
		}
		if (Driver.ReadVirtualMemory<DWORD64>(ProcessId, component, sizeof(DWORD64)) != (ClientAddress + VT_MARKER)) {
			continue;
		}
		std::uint8_t tmp = 1;
		Driver.WriteVirtualMemory64(ProcessId, component + 0x532, *(PULONG64)(&tmp), sizeof(std::uint8_t));
		Driver.WriteVirtualMemory64(ProcessId, component + 0x534, *(PULONG64)(&tmp), sizeof(std::uint8_t));
	}
}

void ESP()
{
	for (DWORD64 idx = 0u; idx <= max_entities(); idx++) {
		DWORD64 entitytmp = entity(idx);
		if (!entitytmp) {
			continue;
		}
		enable_marker(entitytmp);
	}
	printf("ESP\n");
	//Sleep(500);
}

DWORD64 getLocalPlayer()
{
	DWORD64 ProfileManager = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ClientAddress + OFFSET_LOCAL_PLAYER, sizeof(DWORD64));
	DWORD64 temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, ProfileManager + 0x68, sizeof(DWORD64));
	temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp, sizeof(DWORD64));
	DWORD64 LocalPlayer = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0x28, sizeof(DWORD64));
	return LocalPlayer;
}

void norecoil()
{
	DWORD64 LocalPlayer = getLocalPlayer();
	DWORD64 temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, LocalPlayer + 0x78, sizeof(DWORD64));
	temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0xC8, sizeof(DWORD64));
	DWORD64 Weapon = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0x208, sizeof(DWORD64));
	float a = 0.0f;
	Driver.WriteVirtualMemory64(ProcessId, Weapon + 0xB0, *(PULONG64)(&a), sizeof(float));
	//Driver.WriteVirtualMemory64(ProcessId, Weapon + 0x50, *(PULONG64)(&a), sizeof(float));
	printf("norecoil\n");
	//Sleep(500); 
}

void nospread()
{
	DWORD64 LocalPlayer = getLocalPlayer();
	DWORD64 temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, LocalPlayer + 0x78, sizeof(DWORD64));
	temp = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0xC8, sizeof(DWORD64));
	DWORD64 Weapon = Driver.ReadVirtualMemory<DWORD64>(ProcessId, temp + 0x208, sizeof(DWORD64));
	float a = 0.0f;
	//Driver.WriteVirtualMemory64(ProcessId, Weapon + 0xB0, *(PULONG64)(&a), sizeof(float));
	Driver.WriteVirtualMemory64(ProcessId, Weapon + 0x50, *(PULONG64)(&a), sizeof(float));
	printf("nospread\n");
	//Sleep(500);
}

int main()
{
	KeInterface Driver("\\\\.\\mydriverver1");
	ProcessId = Driver.GetTargetPid();
	ClientAddress = Driver.GetClientModule();
	
	std::cout << "Found R6S Process Id: " << ProcessId << std::endl;
	std::cout << "Found R6S ClientBase: 0x" << std::uppercase
		<< std::hex << ClientAddress << std::endl;
	system("pause");

	while (TRUE)
	{
		if ((GetAsyncKeyState(VK_F1) & 0x8000))
			glow(255.0f, 0.0f, 255.0f, 999.0f);
		if ((GetAsyncKeyState(VK_F2) & 0x8000))
			glow(255.0f, 0.0f, 0.0f, 999.0f);
		if ((GetAsyncKeyState(VK_F3) & 0x8000))
			ESP();
		if ((GetAsyncKeyState(VK_F4) & 0x8000))
			norecoil();
		if ((GetAsyncKeyState(VK_F5) & 0x8000))
			nospread();
	}
	Sleep(50);
}

